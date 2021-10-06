#include <control.hpp>

Control::Control(/* args */)
{
}

Control::~Control()
{
}

void Control::add_new_sensors(json config_file)
{
    servers[config_file["nome"]] = {config_file["ip"], config_file["porta"], false, false};

    for (const auto sensor : config_file["outputs"])
        sensors[sensor["gpio"]] = {sensor["type"], sensor["tag"], config_file["nome"], "output", false};

    for (const auto sensor : config_file["inputs"])
        sensors[sensor["gpio"]] = {sensor["type"], sensor["tag"], config_file["nome"], "input", false};
}

void Control::change_sensor_state(int gpio, int state)
{
    auto server = servers[sensors[gpio].server_flag];

    Socket socket(server.ip, server.port);

    auto request = request_toggle({gpio, state});

    socket.send_data(request.to_json());
}

void Control::toggle_sensor(int gpio)
{
    auto server = servers[sensors[gpio].server_flag];

    sensors[gpio].state = not sensors[gpio].state;
}

void Control::lamps_toggle(int gpio)
{
    change_sensor_state(gpio, 1);

    servers[sensors[gpio].server_flag].lamp_timer = true;

    sleep(10);

    if (not servers[sensors[gpio].server_flag].lamp_timer)
        return;

    change_sensor_state(gpio, 0);
}

void Control::play_song(string type)
{
    const char *command;
    bool flag;

    if (type == "alarm")
        flag = alarmed, command = "cd ..; omxplayer ../assets/alarm.mp3 > /dev/null";
    else if (type == "alarm")
        flag = fire_alarmed, command = "cd ..; ../assets/fire.mp3 > /dev/null";

    while (flag)
        system(command);
}

void Control::alarm_control(string type, bool signal)
{
    if (type == "alarm")
        alarmed = signal;

    else if (type == "fire")
        fire_alarmed = signal;

    if (signal)
    {
        thread song_thread(&Control::play_song, this, type);

        song_thread.detach();
    }
}

void Control::control(int gpio)
{
    if (sensors[gpio].mode == "output")
        return;

    if (sensors[gpio].type == "fumaca")
    {
        if (sensors[gpio].state and not fire_alarmed)
        {
            auto sprinkler_gpio = 0;

            for (const auto &sensor : sensors)
                if (sensor.second.type == "aspersor")
                    sprinkler_gpio = sensor.first;

            alarm_control("fire", true);

            change_sensor_state(sprinkler_gpio, true);
        }

        else if (not sensors[gpio].state and fire_alarmed)
        {
            auto sprinkler_gpio = 0;

            for (const auto &sensor : sensors)
                if (sensor.second.type == "aspersor")
                    sprinkler_gpio = sensor.first;

            alarm_control("fire", false);

            change_sensor_state(sprinkler_gpio, false);
        }
    }

    else if (sensors[gpio].type == "presenca")
    {
        auto current_lamp = 0;

        if (sensors[gpio].state)
        {
            if (servers[sensors[gpio].server_flag].alarm_on)
            {
                if (not alarmed)
                    alarm_control("alarm", true);
            }
            else
            {
                for (const auto &sensor : sensors)
                    if (sensor.second.type == "lampada corredor" and sensors[gpio].server_flag == sensor.second.server_flag)
                        current_lamp = sensor.first;

                servers[sensors[gpio].server_flag].lamp_timer = false;

                thread lamp_thread(&Control::lamps_toggle, this, current_lamp);
                lamp_thread.detach();
            }
        }
        else
        {
            if (alarmed)
                alarm_control("alarm", false);
        }
    }
}

void Control::end_servers()
{
    for (auto &server : servers)
    {
        Socket socket(server.second.ip, server.second.port);

        socket.send_data("{\"option\": \"finish_program\"}");
    }
}