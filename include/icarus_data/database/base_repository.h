#ifndef BASE_REPOSITORY_H_
#define BASE_REPOSITORY_H_

#include <iostream>
#include <string>


#include <mysql/mysql.h>

namespace icarus_data::database
{
template<class ConnStr>
class base_repository
{
public:
    base_repository() = delete;
    base_repository(const ConnStr &details, const std::string &table_name) :
        details(details), table_name(table_name)
    {
    }
    base_repository(const std::string &path) :
        path(path)
    {
    }
    base_repository(const std::string &path, std::string &&table_name) :
        path(path), table_name(table_name)
    {
    }
protected:
    bool test_connection()
    {
        auto *conn = setup_connection();

        if(conn == nullptr)
        {
            return false;
        }

        mysql_close(conn);
        
        return true;
    }


    MYSQL *setup_connection()
    {
        auto conn = mysql_init(nullptr);
        const auto server = this->details.host.c_str();
        const auto database = this->details.database.c_str();
        const auto username = this->details.username.c_str();
        const auto password = this->details.password.c_str();

        if(!mysql_real_connect(conn, server, username, password, database, 0, nullptr, 0))
        {
            std::cout << "Failed to connect to the database\n";
        }

        return conn;
    }
    MYSQL_RES *perform_query(MYSQL *conn, const std::string &query)
    {
        // send the query to the database
        if(mysql_query(conn, query.c_str()))
        {
            std::cout << "MySQL query error : " << mysql_error(conn) << "\n";
            std::exit(1);
        }

        return mysql_use_result(conn);
    }


    ConnStr details;
    std::string table_name;
private:
    std::string path;
};
}

#endif
