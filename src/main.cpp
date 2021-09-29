#include <wiringPi.h>

#include <dht22.hpp>

int main(int argc, char const *argv[])
{
    if (wiringPiSetup() == -1)
        throw "Failed to initialize wiringPi\n";

    DHT22 *sensor = new DHT22(28);

    for (const auto &item : sensor->get_temperature())
        cout << item.first << " " << item.second << endl;

    return 0;
}
