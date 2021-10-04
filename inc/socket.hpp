#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>

using namespace std;

class Socket
{
private:
    struct sockaddr_in address;
    int client_socket_id;
    bool server_mode;
    int socket_id;
    string ip;
    int port;

    void setup_server_mode();
    void setup_client_mode();

public:
    Socket(string ip, int port);
    ~Socket();

    int send_data(string message);
    string receive_data();

    void close_client_connection();
};

#endif