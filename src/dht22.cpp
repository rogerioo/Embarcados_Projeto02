#include <dht22.hpp>

DHT22::DHT22(int dht_pin)
{
    if (wiringPiSetup() == -1)
        throw "Failed to initialize wiringPi\n";

    this->dht_pin = dht_pin;
}

map<string, float> DHT22::get_temperature()
{
    map<string, float> output_data{{"temperature", -1}, {"humidity", -1}};

    while (output_data["temperature"] == -1)
    {
        output_data = read_dht_data();
        delay(WAIT_TIME);
    }

    return output_data;
}

map<string, float> DHT22::read_dht_data()
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0;
    uint8_t i;

    int data[5] = {0, 0, 0, 0, 0};

    /* pull pin down for 18 milliseconds */
    pinMode(this->dht_pin, OUTPUT);
    digitalWrite(this->dht_pin, LOW);
    delay(18);

    /* prepare to read the pin */
    pinMode(this->dht_pin, INPUT);

    /* detect change and read data */
    for (i = 0; i < MAX_TIMINGS; i++)
    {
        counter = 0;
        while (digitalRead(this->dht_pin) == laststate)
        {
            counter++;
            delayMicroseconds(1);
            if (counter == 255)
            {
                break;
            }
        }
        laststate = digitalRead(this->dht_pin);

        if (counter == 255)
            break;

        /* ignore first 3 transitions */
        if ((i >= 4) && (i % 2 == 0))
        {
            /* shove each bit into the storage bytes */
            data[j / 8] <<= 1;
            if (counter > 16)
                data[j / 8] |= 1;
            j++;
        }
    }

    /*
	 * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	 * print it out if data is good
	 */

    map<string, float> output_data{{"temperature", -1}, {"humidity", -1}};

    cout << this->dht_pin << " AQUI" << endl;
    cout << data[0] << " " << data[2] << " " << (float)(((data[2] & 0x7F) << 8) + data[3]) / 10 << endl;

    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
    {
        float h = (float)((data[0] << 8) + data[1]) / 10;
        if (h > 100)
        {
            h = data[0]; // for DHT11
        }
        float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (c > 125)
        {
            c = data[2]; // for DHT11
        }
        if (data[2] & 0x80)
        {
            c = -c;
        }
        output_data["temperature"] = c;
        output_data["humidity"] = h;

        cout << "ENTREI" << endl;
    }

    return output_data;
}