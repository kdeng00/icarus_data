#ifndef USERREPOSITORY_H_
#define USERREPOSITORY_H_

#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <mysql/mysql.h>

#include "icarus_data/database/base_repository.h"

namespace icarus_data { namespace database {
template<class User, class PassSec, typename SaltFilter, typename Filter, class ConnStr>
class user_repository : public base_repository<ConnStr>
{
public:
    user_repository(const ConnStr &conn_str) : base_repository<ConnStr>(conn_str)
    {
        this->table_name = "User";
    }


    User retrieveUserRecord(User &usr, Filter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT * FROM User WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        auto userLength = usr.username.size();
        switch (filter) {
        case Filter::id:
            qry << "UserId = ?";

            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = (char*)&usr.id;
            params[0].length = nullptr;
            params[0].is_null = 0;

            break;
        case Filter::username:
            qry << "Username = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)usr.username.c_str();
            params[0].length = &userLength;
            params[0].is_null = 0;
           break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        usr = parseRecord(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return usr;
    }
    PassSec retrieverUserSaltRecord(PassSec &userSec, SaltFilter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT * FROM Salt WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        switch (filter) {
        case SaltFilter::userId:
            qry << "UserId = ?";

            params[0].buffer_type = MYSQL_TYPE_LONG;
            params[0].buffer = (char*)&userSec.user_id;
            params[0].length = 0;
            params[0].is_null = 0;
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        userSec = parseSaltRecord(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return userSec;
    }

    bool doesUserRecordExist(const User &user, Filter filter)
    {
        std::stringstream qry;
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        qry << "SELECT * FROM User WHERE ";

        MYSQL_BIND params[1];
        std::memset(params, 0, sizeof(params));

        auto userLength = user.username.size();
        switch (filter) {
        case Filter::username:
            qry << "Username = ?";

            params[0].buffer_type = MYSQL_TYPE_STRING;
            params[0].buffer = (char*)user.username.c_str();
            params[0].length = &userLength;
            params[0].is_null = 0;
            break;
        default:
            break;
        }

        qry << " LIMIT 1";

        const auto query = qry.str();
        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params);
        status = mysql_stmt_execute(stmt);

        mysql_stmt_store_result(stmt);
        const auto rowCount = mysql_stmt_num_rows(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);

        return (rowCount > 0) ? true : false;
    }

    /**
    bool user_exists(const User &user, Filter filter)
    {
        soci::session conn;
        create_connection(conn);

        std::stringstream qry;
        qry << "SELECT * FROM " << this->table_name << " WHERE ";
        qry << "Username = :username LIMIT 1";

        soci::statement stmt = (conn.prepare << qry.str(), 
                soci::use(user.username, "username"));
        stmt.execute();

        const auto rows = stmt.get_affected_rows();

        conn.close();

        return (rows > 0) ? true : false;
    }
    */

    void saveUserRecord(const User &user)
    {
        std::cout << "inserting user record\n";
        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "INSERT INTO User(Firstname, Lastname, Phone, Email, Username, Password) ";
        qry << "VALUES(?, ?, ?, ?, ?, ?)";

        const auto query = qry.str();
        auto sizes = fetchUserLengths(user);
        auto params = insertUserValues(user, sizes);

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, params.get());
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);
    }
private:
    struct UserLengths;
    struct SaltLengths;

    struct UserLengths {
        unsigned long firstnameLength;
        unsigned long lastnameLength;
        unsigned long phoneLength;
        unsigned long emailLength;
        unsigned long usernameLength;
        unsigned long passwordLength;
    };
    struct SaltLengths {
        unsigned long saltLength;
    };


    void saveUserSalt(const PassSec &userSec)
    {
        std::cout << "inserting user salt record\n";

        auto conn = this->setup_connection();
        auto stmt = mysql_stmt_init(conn);

        std::stringstream qry;
        qry << "INSERT INTO Salt(Salt, UserId) VALUES(?,?)";

        const auto query = qry.str();

        auto sizes = fetchSaltLengths(userSec);
        auto values = insertSaltValues(userSec, sizes);

        auto status = mysql_stmt_prepare(stmt, query.c_str(), query.size());
        status = mysql_stmt_bind_param(stmt, values.get());
        status = mysql_stmt_execute(stmt);

        mysql_stmt_close(stmt);
        mysql_close(conn);
    }


    std::shared_ptr<MYSQL_BIND> insertUserValues(const User &user, 
            std::shared_ptr<UserLengths> lengths)
    {
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(6, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_STRING;
        values.get()[0].buffer = (char*)user.firstname.c_str();
        values.get()[0].length = &(lengths->firstnameLength);
        values.get()[0].is_null = 0;

        values.get()[1].buffer_type = MYSQL_TYPE_STRING;
        values.get()[1].buffer = (char*)user.lastname.c_str();
        values.get()[1].length = &(lengths->lastnameLength);
        values.get()[1].is_null = 0;

        values.get()[2].buffer_type = MYSQL_TYPE_STRING;
        values.get()[2].buffer = (char*)user.phone.c_str();
        values.get()[2].length = &(lengths->phoneLength);
        values.get()[2].is_null = 0;

        values.get()[3].buffer_type = MYSQL_TYPE_STRING;
        values.get()[3].buffer = (char*)user.email.c_str();
        values.get()[3].length = &(lengths->emailLength);
        values.get()[3].is_null = 0;

        values.get()[4].buffer_type = MYSQL_TYPE_STRING;
        values.get()[4].buffer = (char*)user.username.c_str();
        values.get()[4].length = &(lengths->usernameLength);
        values.get()[4].is_null = 0;

        values.get()[5].buffer_type = MYSQL_TYPE_STRING;
        values.get()[5].buffer = (char*)user.password.c_str();
        values.get()[5].length = &(lengths->passwordLength);
        values.get()[5].is_null = 0;

        return values;
    }
    std::shared_ptr<MYSQL_BIND> insertSaltValues(const PassSec &passSec, 
            std::shared_ptr<SaltLengths> lengths)
    {
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(6, sizeof(MYSQL_BIND)));

        values.get()[0].buffer_type = MYSQL_TYPE_STRING;
        values.get()[0].buffer = (char*)passSec.hash_password.c_str();
        values.get()[0].length = &(lengths->saltLength);

        values.get()[1].buffer_type = MYSQL_TYPE_LONG;
        values.get()[1].buffer = (char*)&passSec.user_id;

        return values;
    }
    std::shared_ptr<MYSQL_BIND> valueBind(User &user, 
            std::tuple<char*, char*, char*, char*, char*, char*> &us)
    {
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(7, sizeof(MYSQL_BIND)));
        constexpr auto strLen = 1024;

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = (char*)&user.id;

        values.get()[1].buffer_type = MYSQL_TYPE_STRING;
        values.get()[1].buffer = (char*)std::get<0>(us);
        values.get()[1].buffer_length = strLen;

        values.get()[2].buffer_type = MYSQL_TYPE_STRING;
        values.get()[2].buffer = (char*)std::get<1>(us);
        values.get()[2].buffer_length = strLen;

        values.get()[3].buffer_type = MYSQL_TYPE_STRING;
        values.get()[3].buffer = (char*)std::get<2>(us);
        values.get()[3].buffer_length = strLen;

        values.get()[4].buffer_type = MYSQL_TYPE_STRING;
        values.get()[4].buffer = (char*)std::get<3>(us);
        values.get()[4].buffer_length = strLen;

        values.get()[5].buffer_type = MYSQL_TYPE_STRING;
        values.get()[5].buffer = (char*)std::get<4>(us);
        values.get()[5].buffer_length = strLen;

        values.get()[6].buffer_type = MYSQL_TYPE_STRING;
        values.get()[6].buffer = (char*)std::get<5>(us);
        values.get()[6].buffer_length = strLen;

        return values;
    }
    std::shared_ptr<MYSQL_BIND> saltValueBind(PassSec &userSalt, char *salt)
    {
        std::shared_ptr<MYSQL_BIND> values((MYSQL_BIND*) std::calloc(3, sizeof(MYSQL_BIND)));
        constexpr auto strLen = 1024;

        values.get()[0].buffer_type = MYSQL_TYPE_LONG;
        values.get()[0].buffer = (char*)&userSalt.id;

        values.get()[1].buffer_type = MYSQL_TYPE_STRING;
        values.get()[1].buffer = (char*)salt;
        values.get()[1].buffer_length = strLen;

        values.get()[2].buffer_type = MYSQL_TYPE_LONG;
        values.get()[2].buffer = (char*)&userSalt.user_id;

        return values;
    }
    std::shared_ptr<UserLengths> fetchUserLengths(const User &user)
    {
        auto userLen = std::make_shared<UserLengths>();
        userLen->firstnameLength = user.firstname.size();
        userLen->lastnameLength = user.lastname.size();
        userLen->phoneLength = user.phone.size();
        userLen->emailLength = user.email.size();
        userLen->usernameLength = user.username.size();
        userLen->passwordLength = user.password.size();

        return userLen;
    }
    std::shared_ptr<SaltLengths> fetchSaltLengths(const PassSec &pass_sec)
    {
        auto saltLen = std::make_shared<SaltLengths>();
        saltLen->saltLength = pass_sec.salt.size();

        return saltLen;
    }

    std::tuple<char*, char*, char*, char*, char*, char*> fetchUV()
    {
        char firstname[1024];
        char lastname[1024];
        char phone[1024];
        char email[1024];
        char username[1024];
        char password[1024];

        return std::make_tuple(firstname, lastname, phone, email, username, password);
    }

    User parseRecord(MYSQL_STMT *stmt)
    {
        User user;
        auto usv = fetchUV();

        auto bindedValues = valueBind(user, usv);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        user.firstname = std::get<0>(usv);
        user.lastname = std::get<1>(usv);
        user.email = std::get<2>(usv);
        user.phone = std::get<3>(usv);
        user.username = std::get<4>(usv);
        user.password = std::get<5>(usv);

        return user;
    }

    PassSec parseSaltRecord(MYSQL_STMT *stmt)
    {
        PassSec userSalt;
        char saltKey[1024];

        auto bindedValues = saltValueBind(userSalt, saltKey);
        auto status = mysql_stmt_bind_result(stmt, bindedValues.get());
        status = mysql_stmt_fetch(stmt);

        userSalt.salt = saltKey;

        return userSalt;
    }
};
}}

#endif
