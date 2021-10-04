#include <wiringPi.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <thread>

#include <server.hpp>

using namespace std;

bool end_program = false;

Server *server;

thread *server_run;

void quit(int signal)
{
    end_program = true;

    delete server_run;
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, quit);

    if (argc <= 1)
        cout << "You MUST pass which floor you want to control" << endl,
            cout << "\n(0)ground_floor | (1)first_floor" << endl,
            exit(-1);

    if (wiringPiSetup() == -1)
        throw "Failed to initialize wiringPi\n";

    auto file = strcmp(argv[1], "0") == 0 ? "ground_floor" : "first_floor";

    server = new Server("json/" + string(file) + ".json");

    server_run = new thread(&Server::run, server);

    server_run->join();

    return 0;
}
