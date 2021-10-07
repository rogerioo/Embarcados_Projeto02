#include <gpio.hpp>

#include <iostream>

using void_function = void (*)();

const int MAX{101};

map<int, sensor_info> sensors;
array<void_function, MAX> pin_event_handler;

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

    Socket client_socket(CENTRAL_IP, CENTRAL_PORT);

    client_socket.send_data(info->to_json());

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

GPIO::GPIO(json config_file)
{
    auto start_time = time(0);

    ifstream pfs("json/pins_conversion.json");
    json pins_file = json::parse(pfs);

    for (auto &pin_info : pins_file)
        gpio_to_wiringpi_pin[pin_info["gpio"]] = pin_info["wiringpi"];

    instantiate_callbacks<MAX>();

    for (auto &sensor_info : config_file["outputs"])
    {
        sensors[sensor_info["gpio"]] = {sensor_info["gpio"], sensor_info["tag"], sensor_info["type"], start_time};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], OUTPUT);
        digitalWrite(gpio_to_wiringpi_pin[sensor_info["gpio"]], 0);

        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH,
                    pin_event_handler[sensor_info["gpio"].get<int>()]);
    }

    for (auto &sensor_info : config_file["inputs"])
    {
        if (sensor_info["type"] == "temperatura")
        {
            dht22 = new DHT22(gpio_to_wiringpi_pin[sensor_info["gpio"]]);
            continue;
        }

        sensors[sensor_info["gpio"]] = {sensor_info["gpio"], sensor_info["tag"], sensor_info["type"], start_time};

        pinMode(gpio_to_wiringpi_pin[sensor_info["gpio"]], INPUT);
        digitalWrite(gpio_to_wiringpi_pin[sensor_info["gpio"]], 0);

        wiringPiISR(gpio_to_wiringpi_pin[sensor_info["gpio"]], INT_EDGE_BOTH,
                    pin_event_handler[sensor_info["gpio"].get<int>()]);
    }
}

GPIO::~GPIO()
{
    if (dht22 != nullptr)
        delete dht22;
}

void GPIO::toggle_output_sensor(int gpio, int signal)
{
    pinMode(gpio_to_wiringpi_pin[gpio], OUTPUT);
    digitalWrite(gpio_to_wiringpi_pin[gpio], signal);
}

map<string, float> GPIO::get_temperature()
{
    map<string, float> output;

    if (dht22 != nullptr)
        output = dht22->get_temperature();

    return output;
}