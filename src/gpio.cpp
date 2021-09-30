#include <gpio.hpp>

#include <iostream>

map<int, pair<string, string>> sensors;

GPIO::GPIO(string sensors_json_path)
{
    map<int, int> gpio_to_wiringpi_pin;

    ifstream pfs("json/pins_conversion.json");
    json jf = json::parse(pfs);

    for (auto &pin_info : jf)
        gpio_to_wiringpi_pin[pin_info["gpio"]] = pin_info["wiringpi"];

    ifstream sfs(sensors_json_path);
    jf = json::parse(sfs);

    map<int, void (*)()> handlers{
        {4, handle_event_4},
        {5, handle_event_5},
        {6, handle_event_6},
        {7, handle_event_7},
        {8, handle_event_8},
        {9, handle_event_9},
        {10, handle_event_10},
        {11, handle_event_11},
        {12, handle_event_12},
        {13, handle_event_13},
        {16, handle_event_16},
        {17, handle_event_17},
        {18, handle_event_18},
        {19, handle_event_19},
        {22, handle_event_22},
        {23, handle_event_23},
        {24, handle_event_24},
        {25, handle_event_25},
        {26, handle_event_26},
        {27, handle_event_27}};

    for (auto &sensor_info : jf["outputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"]};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], OUTPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH, handlers[sensor_info["gpio"]]);
    }

    for (auto &sensor_info : jf["inputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"]};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], INPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH, handlers[sensor_info["gpio"]]);
    }
}

GPIO::~GPIO()
{
}

void GPIO::handle_event_base(pair<string, string> info)
{
    cout << info.first << " " << info.second << endl;
}

void GPIO::handle_event_4(void)
{
    auto gpio = 4;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_5(void)
{
    auto gpio = 5;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_6(void)
{
    auto gpio = 6;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_7(void)
{
    auto gpio = 7;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_8(void)
{
    auto gpio = 8;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_9(void)
{
    auto gpio = 9;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_10(void)
{
    auto gpio = 10;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_11(void)
{
    auto gpio = 11;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_12(void)
{
    auto gpio = 12;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_13(void)
{
    auto gpio = 13;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_16(void)
{
    auto gpio = 16;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_17(void)
{
    auto gpio = 17;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_18(void)
{
    auto gpio = 18;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_19(void)
{
    auto gpio = 19;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_22(void)
{
    auto gpio = 22;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_23(void)
{
    auto gpio = 23;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_24(void)
{
    auto gpio = 24;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_25(void)
{
    auto gpio = 25;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_26(void)
{
    auto gpio = 26;

    handle_event_base(sensors[gpio]);
}

void GPIO::handle_event_27(void)
{
    auto gpio = 27;

    handle_event_base(sensors[gpio]);
}

void setup_sensors()
{
}