#ifndef GPIO_HPP
#define GPIO_HPP

#include <nlohmann/json.hpp>
#include <wiringPi.h>

#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#define IGNORE_CHANGE_BELOW_SEC 0.1

using json = nlohmann::json;
using namespace std;

struct sensor_info
{
    string tag;
    string type;
    time_t last_change;

} typedef sensor_info;

extern map<int, sensor_info> sensors;

class GPIO
{
private:
public:
    GPIO(string sensors_json_path);
    ~GPIO();
};

#endif