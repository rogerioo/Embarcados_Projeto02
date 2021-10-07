#ifndef GPIO_HPP
#define GPIO_HPP

#include <nlohmann/json.hpp>
#include <wiringPi.h>

#include <socket.hpp>
#include <dht22.hpp>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#define IGNORE_CHANGE_BELOW_SEC 0.2
#define CENTRAL_IP "192.168.0.53"
#define CENTRAL_PORT 8030

using json = nlohmann::json;
using namespace std;

struct sensor_info
{
    int gpio;
    string tag;
    string type;
    time_t last_change;

    string to_json() const
    {
        ostringstream output;

        output << "{"
               << "\"option\": \"toggle\", "
               << "\"gpio\": " << gpio << ", "
               << "\"tag\": \"" << tag << "\", "
               << "\"type\": \"" << type << "\""
               << "}";

        return output.str();
    }

} typedef sensor_info;

extern map<int, sensor_info> sensors;

class GPIO
{
private:
    map<int, int> gpio_to_wiringpi_pin;
    DHT22 *dht22;

public:
    GPIO(json config_file);
    ~GPIO();

    void toggle_output_sensor(int gpio, int signal);
    map<string, float> get_temperature();
};

#endif