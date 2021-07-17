#include <iostream>

#include "example.hpp"

#include "icarus_data/icarus_data.h"

using std::cout;

using icarus_data::conn_string;
using icarus_data::types::salt_filter;
using icarus_data::types::user_filter;

int main(int argc, char **argv)
{
    const std::string name("fetch_users");

    example::count_check(argc, name);
    const auto conn_str = example::test_connection_string<conn_string>(argv);

    auto user_repo = icarus_data::user_repository(conn_str);

    auto usr = icarus_data::user();
    usr.id = 1;
    usr = user_repo.retrieveUserRecord(usr, user_filter::ID);

    cout << "Username: " << usr.username << "\n";

    auto salt = icarus_data::pass_sec();
    salt.user_id = usr.id;
    salt = user_repo.retrieverUserSaltRecord(salt, salt_filter::ID);

    cout << "Salt: " << salt.salt << "\n";


    return 0;
}