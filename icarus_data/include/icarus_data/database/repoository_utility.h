#ifndef REPO_UTIL_H_
#define REPO_UTIL_H_

#include <iostream>
#include <memory>
#include <cstring>

#include "mysql/mysql.h"

namespace icarus_data { namespace database {

class repository_utility
{
public:
    // long maps to C int
    template<typename Bind, typename MySQLDataType = std::shared_ptr<MYSQL_BIND>, typename type = int>
    static void construct_param_long(Bind params, MySQLDataType buffer_type, const type &buffer, int pos)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)&buffer;
        params.get()[pos].length = 0;
        params.get()[pos].is_null = 0;
    }

    template<typename Bind, typename MySQLDataType = std::shared_ptr<MYSQL_BIND>, typename type = std::string>
    static void construct_param_string(Bind params, MySQLDataType buffer_type, const type &buffer, int pos, long unsigned int &length)
    {
        params.get()[pos].buffer_type = buffer_type;
        params.get()[pos].buffer = (char *)buffer.c_str();
        params.get()[pos].length = &length;
        params.get()[pos].is_null = 0;
    }
};

}}


#endif
