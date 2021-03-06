#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <iostream>

#include "icarus_data/icarus_data.h"

using icarus_data::conn_string;

namespace example {

template<typename ConnString>
ConnString test_connection_string(char **argv)
{
    auto conn_str = ConnString();
    conn_str.host = argv[1];
    conn_str.database = argv[2];
    conn_str.username = argv[3];
    conn_str.password = argv[4];

    return conn_str;
}

void count_check(int argc, const std::string &name)
{
    std::cout << name << "\n";

    if (argc < 5)
    {
        std::cout << "Pass database connection details like so:\n";
        std::cout << "./" << name << " [host] [database] [username] [password]\n";
        std::cout << "Terminating...\n";

        std::terminate();
    }
}

}

#endif