#ifndef AUTH_H_
#define AUTH_H_


namespace icarus_data { namespace models {

template<typename Str, typename Dur>
class token
{
public:
    token() = default;

    Str access_token;
    Dur expiration;
    Str token_type;
};


template<typename Str>
class login_result
{
public:
    int user_dd;
    Str username;
    Str access_token;
    Str token_type;
    Str message;
    int expiration;
};

template<typename Str>
class register_result {
public:
    Str username;
    // TODO: Look into another way of expressing registration without using a bool
    bool registered;
    Str message;
};

template<typename Str>
class user
{
public:
    int id;
    Str firstname;
    Str lastname;
    Str email;
    Str phone;
    Str username;
    Str password;
};

template<typename Str>
class pass_sec {
public:

    int id;
    Str hash_password;
    Str salt;
    int user_id;
};

}}

#endif
