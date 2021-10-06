#include <signal.h>

#include <thread>

#include <control.hpp>
#include <server.hpp>

using namespace std;

bool end_program = false;

thread *server_socket;

Server *server;
Control control;

void quit(int signal)
{
    end_program = true;

    Socket socket("127.0.0.1", SERVER_PORT);
    socket.send_data("{\"option\": \"finish_program\"}");

    server_socket->join();

    delete server_socket;

    delete server;

    exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, quit);

    server = new Server();

    server_socket = new thread(&Server::start, server);

    server_socket->join();

    return 0;
}
