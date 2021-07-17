#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "icarus_data/icarus_data.h"

using std::cout;
using std::vector;

using song = icarus_data::song;
using conn_string = icarus_data::conn_string;
using song_repository = icarus_data::song_repository;


conn_string test_connection_string(char **argv)
{
    auto conn_str = conn_string();
    conn_str.host = argv[1];
    conn_str.database = argv[2];
    conn_str.username = argv[3];
    conn_str.password = argv[4];

    return conn_str;
}

template<typename Song, typename Con>
void print_songs(Con songs)
{
    if (songs.size() > 0)
    {
        cout << "Printing songs\n\n";

        std::for_each(songs.begin(), songs.end(), [](song song)
        {
            cout << "Title: " << song.title << "\n";
            cout << "Artist: " << song.artist << "\n";
            cout << "Album: " << song.album << "\n";
            cout << "Album Artist: " << song.album_artist << "\n";
            cout << "Genre: " << song.genre << "\n";
            cout << "Year: " << song.year << "\n";
            cout << "Duration: " << song.duration << "\n";
            cout << "Track: " << song.track << "\n";
            cout << "Disc: " << song.disc << "\n";
            cout << "\n";
        });
    }
    else
    {
        cout << "No songs to print\n";
    }
}

int main(int argc, char **argv)
{
    cout << "fetch_songs\n";

    if (argc < 5)
    {
        cout << "Pass database connection details like so:\n";
        cout << "[host] [database] [username] [password]\n";

        return -1;
    }

    const auto conn_str = test_connection_string(argv);

    auto song_repo = song_repository(conn_str);

    const auto songs = song_repo.retrieveRecords();
    print_songs<song, vector<song>>(songs);


    return 0;
}
