#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <map>
#include <string>
#include <iostream>

#define MAX_TIMINGS 85
#define DEBUG 0
#define WAIT_TIME 2000

using namespace std;

class DHT22
{
private:
    int dht_pin;

    map<string, float> read_dht_data();

public:
    DHT22(int dht_pin);
    // ~DHT22();

    map<string, float> get_temperature();
};
