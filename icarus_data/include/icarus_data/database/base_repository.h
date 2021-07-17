#ifndef BASE_REPOSITORY_H_
#define BASE_REPOSITORY_H_

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <memory>
#include <vector>


#include <mysql/mysql.h>
#include <soci/soci.h>
#include <soci/soci-backend.h>
// #include <soci/mysql/soci-mysql.h>

namespace icarus_data { namespace database {
template<class ConnStr>
class base_repository
{
protected:
    base_repository() = delete;
    base_repository(const ConnStr &details) : details(details)
    {
    }
    base_repository(const ConnStr &details, const std::string &table_name) :
        details(details), table_name(table_name)
    {
    }
    base_repository(const std::string &path) :
        path(path)
    {
    }

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

    void create_connection(soci::session &conn)
    {
        auto conn_string = generate_connection_string();
        conn.open("mysql", conn_string);
    }


    std::string generate_connection_string() noexcept
    {
        std::stringstream qry;
        qry << "db=" << this->details.database << " user=";
        qry << this->details.username << " host=" << this->details.server;
        qry << " password='" << this->details.password << "'";

        return qry.str();
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

    /**
    template<typename MySQLDataType = enum_field_types, typename type = int>
    MYSQL_BIND construct_param(MySQLDataType buffer_type, const type buffer)
    {
        MYSQL_BIND param;
        // std::memset(param, 0, sizeof(param));
        //
        param.buffer_type = buffer_type;
        //
        switch (buffer_type)
        {
            case MYSQL_TYPE_LONG:
                param.buffer = (char *)&buffer;
                param.length = 0;
                param.is_null = 0;
                break;
            case MYSQL_TYPE_STRING:
                param.buffer_type = buffer_type;
                // param.buffer = (char *)buffer.c_str();
                // param.length = &buffer.size();
                param.is_null = 0;
            default:
                break;
        }

        return param;
    }
    */

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
}}

#endif
