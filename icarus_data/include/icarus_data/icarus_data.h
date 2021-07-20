#ifndef ICARUS_DATA_H_
#define ICARUS_DATA_H_

#include "icarus_data/database/album_repository.h"
#include "icarus_data/database/artist_repository.h"
#include "icarus_data/database/cover_art_repository.h"
#include "icarus_data/database/genre_repository.h"
#include "icarus_data/database/song_repository.h"
#include "icarus_data/database/token_repository.h"
#include "icarus_data/database/user_repository.h"
#include "icarus_data/database/year_repository.h"

#include "icarus_data/models/auth.hpp"
#include "icarus_data/models/connection_string.hpp"
#include "icarus_data/models/music.hpp"
#include "icarus_data/types/auth.hpp"
#include "icarus_data/types/music.hpp"

#include <string>

// TODO: Move the filters to this library

namespace icarus_data
{
    // icarus_data data type. Used to represent data
    using id_data_type = unsigned char;

    using login_result = models::login_result<std::string>;
    using pass_sec = models::pass_sec<std::string>;
    using register_result = models::register_result<std::string>;
    using token = models::token<std::string, long>;
    using user = models::user<std::string>;

    using conn_string = models::connection_string<std::string>;

    using album = models::album<std::string>;
    using artist = models::artist<std::string>;
    using cover_art = models::cover<std::string, id_data_type, std::vector<id_data_type>>;
    using genre = models::genre<std::string>;
    using song = models::song<std::string, id_data_type, conn_string>;

    using song_filter = types::song_filter;
    using artist_filter = types::artist_filter;
    using album_filter = types::album_filter;
    using genre_filter = types::genre_filter;
    using cover_art_filter = types::cover_filter;
    using token_filter = types::token_filter;
    using user_filter = types::user_filter;
    using salt_filter = types::salt_filter;

    using album_repository = database::album_repository<album, types::album_filter, conn_string>;
    using artist_repository = database::artist_repository<artist, types::artist_filter, conn_string>;
    using cover_art_repository = database::cover_art_repository<cover_art, types::cover_filter, conn_string>;
    using genre_repository = database::genre_repository<genre, types::genre_filter, conn_string>;
    using song_repository = database::song_repository<song, types::song_filter, conn_string>;
    using token_repository = database::token_repository<token, types::token_filter, conn_string>;
    using user_repository = database::user_repository<user, pass_sec, 
        types::salt_filter, types::user_filter, conn_string>;
}


#endif
