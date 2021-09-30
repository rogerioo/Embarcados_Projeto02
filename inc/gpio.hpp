#ifndef GPIO_HPP
#define GPIO_HPP

#include <nlohmann/json.hpp>
#include <wiringPi.h>

#include <fstream>
#include <vector>
#include <string>
#include <map>

using json = nlohmann::json;
using namespace std;

extern map<int, pair<string, string>> sensors;

class GPIO
{
private:
public:
    GPIO(string sensors_json_path);
    ~GPIO();
};

#endif