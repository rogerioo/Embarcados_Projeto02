#include <server.hpp>

Server::Server(string config_file_path)
{
    gpio = new GPIO(config_file_path);

    socket = new Socket("", 10126);

    dht22 = new DHT22(28);
}

Server::~Server()
{
    delete socket;
    delete dht22;
    delete gpio;
}

void Server::run()
{
    while (not end_program)
    {
        string in_data = socket->receive_data();

        json request = json::parse(in_data);

        if (request["option"] == "toggle")
            gpio->toggle_output_sensor(request["gpio"], request["signal"]);

        else if (request["option"] == "get_temperature")
        {
            auto data = dht22->get_temperature();

            json response(data);

            socket->send_data(response.dump());
        }

        socket->close_client_connection();
    }
}