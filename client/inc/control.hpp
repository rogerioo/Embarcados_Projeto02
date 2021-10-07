#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <nlohmann/json.hpp>

#include <socket.hpp>

#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <map>

using namespace std;
using json = nlohmann::json;

extern bool end_program;

struct sensor_info
{
    string type;
    string tag;
    string server_flag;
    string mode;
    bool state;

    string to_json() const
    {
        ostringstream output;

        output << "{"
               << "\"tag:\"" << tag << ", "
               << "\"type:\"" << type
               << "}";

        return output.str();
    }

} typedef sensor_info;

struct server_info
{
    string ip;
    int port;
    bool lamp_timer;
} typedef server_info;

class Control
{
private:
    map<string, server_info> servers;
    map<int, sensor_info> sensors;

    map<string, map<string, float>> temperatures;

    bool fire_alarmed;
    bool alarmed;
    bool alarm_mode;

    int people_amount;

    void alarm_control(string type, bool signal);
    void lamps_toggle(int gpio);
    void play_song(string type);

public:
    Control(/* args */);
    ~Control();

    void add_new_sensors(json config_file);
    void toggle_sensor(int gpio);
    void change_sensor_state(int gpio, int state);
    void control(int gpio);
    void end_servers();

    void get_temperatures_deamon();

    string get_temperature_info(string server_flag);
    map<int, sensor_info> get_sensors_info();
    string get_people_amount();
    string get_alarm_mode();
};

#endif