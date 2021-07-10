#ifndef CONNECTION_STRING_H_
#define CONNECTION_STRING_H_


namespace icarus_data { namespace models {

template<typename Str>
class connection_string
{
public:
    Str host;
    Str database;
    Str username;
    Str password;
    int port;
    Str Driver;
private:
};
}}

#endif
