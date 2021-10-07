#include <signal.h>

#include <thread>

#include <control.hpp>
#include <server.hpp>
#include <screen.hpp>

using namespace std;

bool end_program = false;

thread *screen_menu_socket;
thread *screen_update_socket;
thread *get_temperature_socket;
thread *server_socket;

Server *server;
Control control;
Screen *screen;

void quit(int signal)
{
    end_program = true;

    Socket socket("127.0.0.1", SERVER_PORT);
    socket.send_data("{\"option\": \"finish_program\"}");

    server_socket->join();
    screen_update_socket->join();
    screen_menu_socket->join();
    get_temperature_socket->join();

    delete get_temperature_socket;
    delete screen_update_socket;
    delete screen_menu_socket;
    delete server_socket;

    delete server;
    delete screen;

    exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, quit);

    server = new Server();

    screen = new Screen();

    server_socket = new thread(&Server::start, server);

    screen_menu_socket = new thread(&Screen::menu_deamon, screen);

    screen_update_socket = new thread(&Screen::data_update_deamon, screen);

    get_temperature_socket = new thread(&Control::get_temperatures_deamon, &control);

    server_socket->join();

    return 0;
}
