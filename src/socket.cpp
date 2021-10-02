#include <socket.hpp>

#include <iostream>

Socket::Socket(string ip, int port) : address({0}), server_mode(ip == "" ? true : false), socket_id(0), ip(ip), port(port)
{
    if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (server_mode)
        setup_server_mode();
    else
        setup_client_mode();
}

Socket::~Socket()
{
    close(socket_id);
}

void Socket::setup_server_mode()
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(socket_id, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_id, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void Socket::setup_client_mode()
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_port = htons(port);

    if (connect(socket_id, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Error: Connection failed\n");
        exit(1);
    }
}

string Socket::receive_data()
{
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int client_socket_id;
    int read_bytes;

    if ((client_socket_id = accept(socket_id, (struct sockaddr *)&address,
                                   (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if ((read_bytes = read(client_socket_id, buffer, 1024)) <= 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buffer);

    close(client_socket_id);

    string output(buffer);

    return output;
}

int Socket::send_data(string message)
{
    if (send(socket_id, (const void *)message.c_str(), message.length(), 0) != (ssize_t)message.length())
    {
        printf("Error: Send failed\n");
        return 1;
    }

    return 0;
}