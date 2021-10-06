#ifndef SERVER_HPP
#define SERVER_HPP

#include <control.hpp>
#include <socket.hpp>

#include <iostream>

#define SERVER_PORT 8030

using namespace std;

extern bool end_program;
extern Control control;

class Server
{
private:
    Socket *socket;

public:
    Server();
    ~Server();

    void start();
};

#endif