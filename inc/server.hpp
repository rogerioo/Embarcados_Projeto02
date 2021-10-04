#ifndef SERVER_HPP
#define SERVER_HPP

#include <nlohmann/json.hpp>

#include <socket.hpp>
#include <dht22.hpp>
#include <gpio.hpp>

using json = nlohmann::json;
using namespace std;

extern bool end_program;

class Server
{
private:
    Socket *socket;
    DHT22 *dht22;
    GPIO *gpio;

public:
    Server(string config_file_path);
    ~Server();

    void run();
};

#endif