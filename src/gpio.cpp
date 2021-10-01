#include <gpio.hpp>

#include <iostream>

using void_function = void (*)();

const int MAX{101};

map<int, pair<string, string>> sensors;
static array<void_function, MAX> pin_event_handler;

void handle_event(pair<string, string> info)
{
    cout << info.first << " " << info.second << endl;
}

template <int pin>
auto callback_generator()
{
    return []()
    { handle_event(sensors[pin]); };
}

template <int N>
void instantiate_callbacks()
{
    if constexpr (N > 0)
    {
        pin_event_handler[N] = callback_generator<N>();
        instantiate_callbacks<N - 1>();
    }
    else
        pin_event_handler[0] = callback_generator<0>();
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

    instantiate_callbacks<MAX>();

    for (auto &sensor_info : jf["outputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"]};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], OUTPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH,
                    pin_event_handler[sensor_info["gpio"].get<int>()]);
    }

    for (auto &sensor_info : jf["inputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"]};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], INPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH,
                    pin_event_handler[sensor_info["gpio"].get<int>()]);
    }
}

GPIO::~GPIO()
{
}
