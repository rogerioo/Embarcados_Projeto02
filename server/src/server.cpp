#include <server.hpp>

Server::Server(string config_file_path)
{
    ifstream sfs(config_file_path);
    json config_file = json::parse(sfs);

    Socket socket_init(CENTRAL_IP, CENTRAL_PORT);
    socket_init.send_data("{\"option\": \"start_program\", \"config_file\": " + config_file.dump() + " }");

    gpio = new GPIO(config_file);

    socket = new Socket("", config_file["porta"]);
}

Server::~Server()
{
    delete socket;
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
            auto data = gpio->get_temperature();

            json response(data);

            socket->send_data(response.dump());
        }
        else if (request["option"] == "finish_program")
        {
            socket->close_client_connection();
            kill(getppid(), SIGINT);
            sleep(2);
            break;
        }

        socket->close_client_connection();
    }
}