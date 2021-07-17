#include <iostream>
#include <string>
#include <algorithm>

#include "icarus_data/icarus_data.h"

using std::cout;


icarus_data::conn_string test_connection_string()
{
    auto conn_str = icarus_data::conn_string();

    return conn_str;
}

int main(int argc, char **argv)
{
    cout << "fetch_songs\n";

    const auto conn_str = test_connection_string();

    auto token_repo = icarus_data::token_repository(conn_str);
    auto song_repo = icarus_data::song_repository(conn_str);

    const auto songs = song_repo.retrieveRecords();

    std::for_each(songs.begin(), songs.end(), [](icarus_data::song song)
    {
        cout << song.title << "\n";
    });


    return 0;
}
