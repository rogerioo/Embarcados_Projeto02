#include <wiringPi.h>

#include <dht22.hpp>

int main(int argc, char const *argv[])
{

    DHT22 *sensor = new DHT22(20);

    for (const auto &item : sensor->get_temperature())
        cout << item.first << " " << item.second << endl;

    return 0;
}
