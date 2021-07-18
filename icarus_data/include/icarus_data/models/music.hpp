#ifndef MUSIC_H_
#define MUSIC_H_

#include <vector>

namespace icarus_data { namespace models {

// TODO: Might need to add some more constructors

template<typename Str, typename Data, typename Con>
class song
{
public:
    song() = default;
    song(const int id) : id(id) { }
    song(const int id, const std::string& title, const std::string& artist,
            const std::string& album, const std::string& album_artist,
            const std::string& genre, const int year, const int duration,
            const int track, const int disc, const std::string song_path) :
            id(id), title(title), artist(artist), album(album),
            album_artist(album_artist), genre(genre), year(year),
            duration(duration), track(track), disc(disc), 
            song_path(song_path) { }


    int id;
    Str title;
    Str artist;
    Str album;
    Str album_artist;
    Str genre;
    int year;
    long duration;
    int track;
    int disc;
    Str song_path;
    Con data;
    int cover_art_id;
    int artist_id;
    int album_id;
    int genre_id;
    int year_id;
};

template<typename Str>
class artist
{
public:
    artist() = default;

    int id;
    Str name;
};


template<typename Str>
class album
{
public:
    album() = default;

    int id;
    Str title;
    Str artist;
    int year;
    // TODO: Add container for songs
};

template<typename Str>
class genre
{
public:
    genre() = default;

    int id;
    Str category;
};

// NOTE: Might get rid of the Year class and the year id field from song.

template<typename Str, typename Data, typename Con>
class cover
{
public:
    cover() = default;

    int id;
    Str song_title;
    Str image_path;
    Con data;
};
}}

#endif