#include <server.hpp>

Server::Server()
{
    socket = new Socket("", SERVER_PORT);
}

Server::~Server()
{
    delete socket;
}

void Server::start()
{
    while (not end_program)
    {
        auto in_data = socket->receive_data();

        cout << in_data << endl;

        auto request = json::parse(in_data);

        if (request["option"] == "toggle")
        {
            control.toggle_sensor(request["gpio"]);
            control.control(request["gpio"]);
        }
        else if (request["option"] == "start_program")
            control.add_new_sensors(request["config_file"]);

        else if (request["option"] == "finish_program")
        {
            control.end_servers();
            socket->close_client_connection();
            break;
        }

        socket->close_client_connection();
    }
}
