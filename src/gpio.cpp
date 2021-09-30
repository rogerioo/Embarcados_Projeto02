#include <gpio.hpp>

#include <iostream>

using fn = void (*)();

const int MAX{101};

map<int, pair<string, string>> sensors;
static array<fn, MAX> Fs;

#define TXT(x) handle_event_##x

void handle_event_base(pair<string, string> info)
{
    cout << info.first << " " << info.second << endl;
}

template <int K>
auto genF()
{
    return []()
    { handle_event_base(sensors[K]); };
}

template <int N>
void buildFs()
{
    if constexpr (N > 0)
    {
        Fs[N] = genF<N>();
        buildFs<N - 1>();
    }
    else
        Fs[0] = genF<0>();
}

GPIO::GPIO(string sensors_json_path)
{
    map<int, int> gpio_to_wiringpi_pin;

    ifstream pfs("json/pins_conversion.json");
    json jf = json::parse(pfs);

    for (auto &pin_info : jf)
        gpio_to_wiringpi_pin[pin_info["gpio"]] = pin_info["wiringpi"];

    ifstream sfs(sensors_json_path);
    jf = json::parse(sfs);

    buildFs<MAX>();

    for (auto &sensor_info : jf["outputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"]};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], OUTPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH, Fs[sensor_info["gpio"].get<int>()]);
    }

    for (auto &sensor_info : jf["inputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"]};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], INPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH, Fs[sensor_info["gpio"].get<int>()]);
    }
}

GPIO::~GPIO()
{
}
