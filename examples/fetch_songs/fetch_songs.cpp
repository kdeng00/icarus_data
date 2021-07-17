#include <iostream>
#include <string>

#include "icarus_data/icarus_data.h"

using std::cout;


icarus_data::conn_string test_connection_string()
{
    auto conn_str = icarus_data::conn_string();
    conn_str.host = "";
    conn_str.database = "";
    conn_str.username = "";
    conn_str.password = "";
    conn_str.port = 3306;
    conn_str.driver = "";

    return conn_str;
}

int main(int argc, char **argv)
{
    cout << "fetch_songs\n";

    const auto conn_str = test_connection_string();

    auto token_repo = icarus_data::token_repository(conn_str);

    return 0;
}
