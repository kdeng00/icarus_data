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
    using token = models::token<std::string, long>;
    using login_result = models::login_result<std::string>;
    using conn_string = models::connection_string<std::string>;

    using song = models::song<std::string, unsigned char, conn_string>;

    using token_repository = database::token_repository<token, types::token_filter, conn_string>;
    using song_repository = database::song_repository<song, types::song_filter, conn_string>;
}


#endif
