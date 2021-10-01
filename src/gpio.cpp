#include <gpio.hpp>

#include <iostream>

using void_function = void (*)();

const int MAX{101};

map<int, sensor_info> sensors;
static array<void_function, MAX> pin_event_handler;

bool is_in(const string &v, initializer_list<string> lst)
{
    return find(begin(lst), end(lst), v) != end(lst);
}

void handle_event(sensor_info *info)
{
    auto now = time(0);
    auto diff = difftime(now, info->last_change);

    info->last_change = now;

    if (diff < IGNORE_CHANGE_BELOW_SEC)
        return;

    printf("%.4lf\n", diff);

    if (is_in(info->type, {"lampada", "ar-condicionado", "aspersor"}))
    {
        //

        return;
    }

    cout << info->tag << " " << info->type << endl;
}

template <int pin>
auto callback_generator()
{
    return []()
    { handle_event(&sensors[pin]); };
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
    auto start_time = time(0);

    ifstream pfs("json/pins_conversion.json");
    json jf = json::parse(pfs);

    for (auto &pin_info : jf)
        gpio_to_wiringpi_pin[pin_info["gpio"]] = pin_info["wiringpi"];

    ifstream sfs(sensors_json_path);
    jf = json::parse(sfs);

    instantiate_callbacks<MAX>();

    for (auto &sensor_info : jf["outputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"], start_time};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], OUTPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH,
                    pin_event_handler[sensor_info["gpio"].get<int>()]);
    }

    for (auto &sensor_info : jf["inputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["tag"], sensor_info["type"], start_time};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], INPUT);
        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH,
                    pin_event_handler[sensor_info["gpio"].get<int>()]);
    }
}

GPIO::~GPIO()
{
}
