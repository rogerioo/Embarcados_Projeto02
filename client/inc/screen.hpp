#ifndef SCREEN_HPP

#define SCREEN_HPP

#define ENTER_KEY 10

#include <control.hpp>

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <menu.h>

#include <algorithm>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

using namespace std;

extern bool end_program;

extern Control control;

class Screen
{
private:
    WINDOW *box_ground_temperature;
    WINDOW *box_first_temperature;
    WINDOW *box_people_counter;
    WINDOW *box_alarm_mode;
    WINDOW *box_menu;
    WINDOW *box_input;
    WINDOW *box_alarms;
    WINDOW *box_input_devices;
    WINDOW *box_output_devices;

    MENU *menu_controller;
    ITEM **menu_items;

    vector<string> menu_options;

    int max_height, max_width;

    void set_menu();
    void set_header();
    void set_input_box();
    void set_devices();
    void set_alarms();

    void set_input_mode(vector<string> message, int option);

    void set_wrong_input_message(string message, int window_height);

public:
    Screen();
    ~Screen();

    void menu_deamon();
    void data_update_deamon();
};

#endif