#ifndef CONNECTION_STRING_H_
#define CONNECTION_STRING_H_


#include <string>

namespace icarus_data::models
{
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
}

#endif
