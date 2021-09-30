#include <wiringPi.h>
#include <unistd.h>

#include <iostream>

#include <dht22.hpp>
#include <gpio.hpp>

GPIO *gpio;

int main(int argc, char const *argv[])
{
    if (argc <= 1)
        cout << "You MUST pass which floor you want to control" << endl,
            cout << "\n(0)ground_floor | (1)first_floor" << endl,
            exit(-1);

    if (wiringPiSetup() == -1)
        throw "Failed to initialize wiringPi\n";

    auto file = strcmp(argv[1], "0") == 0 ? "ground_floor" : "first_floor";

    DHT22 *sensor = new DHT22(28);
    gpio = new GPIO("json/" + string(file) + ".json");

    for (const auto &item : sensor->get_temperature())
        cout << item.first << " " << item.second << endl;

    while (1)
        sleep(1);

    return 0;
}
