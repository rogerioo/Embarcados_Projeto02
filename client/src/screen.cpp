#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include "screen.hpp"

#include <iostream>

using namespace std;

Screen::Screen() : box_ground_temperature(nullptr),
                   box_first_temperature(nullptr),
                   box_people_counter(nullptr),
                   box_alarm_mode(nullptr),
                   box_menu(nullptr),
                   box_input(nullptr),
                   box_input_devices(nullptr),
                   box_output_devices(nullptr),
                   menu_controller(nullptr),
                   menu_items(nullptr),
                   menu_options({"1. Trigger device",
                                 "2. Turn ON/OFF building security mode",
                                 "3. Turn ON/OFF building fire alarm",
                                 "4. Exit system"})
{
    try
    {
        initscr();
        noecho();
        curs_set(false);

        getmaxyx(stdscr, max_height, max_width);

        set_header();
        set_menu();
        set_input_box();
        set_devices();

        wrefresh(box_ground_temperature);
        wrefresh(box_first_temperature);
        wrefresh(box_people_counter);
        wrefresh(box_alarm_mode);

        wrefresh(box_menu);
        wrefresh(box_input);

        wrefresh(box_input_devices);
        wrefresh(box_output_devices);
    }
    catch (...)
    {
        delwin(box_ground_temperature);
        delwin(box_first_temperature);
        delwin(box_people_counter);
        delwin(box_alarm_mode);
        delwin(box_input);

        unpost_menu(menu_controller);
        free_menu(menu_controller);
        delwin(box_menu);

        endwin();
    }
}

Screen::~Screen()
{
    delwin(box_ground_temperature);
    delwin(box_first_temperature);
    delwin(box_people_counter);
    delwin(box_alarm_mode);
    delwin(box_input);

    unpost_menu(menu_controller);
    free_menu(menu_controller);
    delwin(box_menu);

    endwin();
}

void Screen::set_devices()
{
    string title_input{" INPUT DEVICES "};
    string title_output{" OUPUT DEVICES "};

    box_input_devices = newwin(max_height - 15, (max_width / 2) - 1, 15, 0);
    box_output_devices = newwin(max_height - 15, (max_width / 2) - 1, 15, max_width / 2);

    box(box_input_devices, 0, 0);
    box(box_output_devices, 0, 0);

    mvwprintw(box_input_devices, 0, (max_width / 4) - title_input.size() / 2, title_input.c_str());
    mvwprintw(box_output_devices, 0, (max_width / 4) - title_output.size() / 2, title_output.c_str());
}

void Screen::set_header()
{
    string title_et{" GROUND FLOOR TEMPERATURE / HUMIDITY "};
    string title_it{" FIRST FLOOR TEMPERATURE / HUMIDITY "};
    string title_rt{" PEOPLE "};
    string title_cm{" ALARM MODE "};

    box_ground_temperature = newwin(5, (max_width / 4) - 1, 0, 0);
    box_first_temperature = newwin(5, (max_width / 4) - 1, 0, max_width / 4);
    box_people_counter = newwin(5, (max_width / 4) - 1, 0, (max_width / 4) * 2);
    box_alarm_mode = newwin(5, (max_width / 4) - 1, 0, (max_width / 4) * 3);

    box(box_ground_temperature, 0, 0);
    box(box_first_temperature, 0, 0);
    box(box_people_counter, 0, 0);
    box(box_alarm_mode, 0, 0);

    mvwprintw(box_ground_temperature, 0, (max_width / 4) / 2 - title_et.size() / 2, title_et.c_str());
    mvwprintw(box_first_temperature, 0, (max_width / 4) / 2 - title_it.size() / 2, title_it.c_str());
    mvwprintw(box_people_counter, 0, (max_width / 4) / 2 - title_rt.size() / 2, title_rt.c_str());
    mvwprintw(box_alarm_mode, 0, (max_width / 4) / 2 - title_cm.size() / 2, title_cm.c_str());
}

void Screen::set_input_box()
{
    string title{" INPUT "};

    box_input = newwin(8, (max_width / 2) - 1, 6, max_width / 2);

    box(box_input, 0, 0);

    mvwprintw(box_input, 0, (max_width / 4) - title.size() / 2, title.c_str());
}

void Screen::set_menu()
{
    string right_padding((max_width / 2) - 4, ' ');
    string title{" MENU "};

    for (size_t i = 0; i < menu_options.size() - 1; i++)
        menu_options[i].append(right_padding);

    keypad(stdscr, TRUE);

    box_menu = newwin(8, (max_width / 2) - 1, 6, 0);

    box(box_menu, 0, 0);
    mvwprintw(box_menu, 0, (max_width / 2) / 2 - title.size() / 2, title.c_str());

    /* Create items */
    menu_items = (ITEM **)calloc(menu_options.size(), sizeof(ITEM *));
    for (size_t i = 0; i < menu_options.size(); ++i)
        menu_items[i] = new_item(menu_options[i].c_str(), "");

    // item_opts_off(menu_items[4], O_SELECTABLE);
    // item_opts_off(menu_items[5], O_SELECTABLE);

    /* Crate menu */
    menu_controller = new_menu((ITEM **)menu_items);

    /* Create the window to be associated with the menu */
    keypad(box_menu, TRUE);

    /* Set main window and sub window */
    set_menu_win(menu_controller, box_menu);
    set_menu_sub(menu_controller, subwin(box_menu, 6, (max_width / 2) - 4, 8, 1));

    /* Set menu mark to the string " * " */
    set_menu_mark(menu_controller, " > ");

    /* Post the menu */
    post_menu(menu_controller);
}

void Screen::menu_deamon()
{

    // wrefresh(box_menu);

    int user_input;

    while (not end_program)
    {
        user_input = wgetch(box_menu);
        switch (user_input)
        {
        case KEY_DOWN:
            menu_driver(menu_controller, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu_controller, REQ_UP_ITEM);
            break;
        case ENTER_KEY:
            auto selected_item = item_name(current_item(menu_controller));

            switch (*selected_item)
            {
            case '1':
                set_input_mode({"Enter a valid GPIO sensor number:"},
                               1);
                break;
            case '2':
                set_input_mode({"Enter the Temperature"}, 2);
                break;
            case '3':
                set_input_mode({"Enter the Hysterese"}, 3);
                break;
            case '4':
                mvwprintw(box_input, 2, 3, "System shutting down...");
                mvwprintw(box_input, 4, 3, "Please wait!!");
                wrefresh(box_input);

                kill(getpid(), SIGINT);
                break;
            }

            break;
        }
        wrefresh(box_menu);
    }
}

void Screen::set_input_mode(vector<string> message, int option)
{
    int window_height, window_width;

    int start_y = 2;

    getmaxyx(box_input, window_height, window_width);

    echo();

    for (size_t i = 0; i < message.size(); i++)
    {
        if (message[i] == "")
            continue;
        mvwprintw(box_input, start_y + i, 3, message[i].c_str());
    }

    wrefresh(box_input);

    wmove(box_input, message.size() + 3, 3);

    int tmp_int;

    switch (option)
    {
    case 1:
        wscanw(box_input, "%d", &tmp_int);

        // if (find(output_devices.begin(), output_devices.end(), box_input) == output_devices.end() and
        //     find(input_devices.begin(), input_devices.end(), box_input) == input_devices.end())
        // {
        //     set_wrong_input_message("Wrong option.", window_height);
        //     break;
        // }

        break;

    default:
        break;
    }

    noecho();

    string clear_string(window_width - 2, ' ');

    for (size_t i = 0; i < message.size() + 2; i++)
    {
        mvwprintw(box_input, 1 + i, 1, clear_string.c_str());
    }

    mvwprintw(box_input, message.size() + 3, 3, "         ");

    wrefresh(box_input);
}

void Screen::set_wrong_input_message(string message, int window_height)
{
    string clean_message(message.size(), ' ');

    mvwprintw(box_input, window_height - 4, 3, message.c_str());
    mvwprintw(box_input, window_height - 2, 3, "This change won't take effect!");
    wrefresh(box_input);
    sleep(2);

    mvwprintw(box_input, window_height - 4, 3, clean_message.c_str());
    mvwprintw(box_input, window_height - 2, 3, "                              ");
}

void Screen::data_update_deamon()
{
    int box_alarm_mode_height, box_alarm_mode_width;
    int box_reference_height, box_reference_width;
    int box_external_height, box_external_width;
    int box_internal_height, box_internal_width;
    int box_input_height, box_input_width;
    int box_output_height, box_output_width;

    getmaxyx(box_people_counter, box_reference_height, box_reference_width);
    getmaxyx(box_alarm_mode, box_alarm_mode_height, box_alarm_mode_width);
    getmaxyx(box_ground_temperature, box_external_height, box_external_width);
    getmaxyx(box_first_temperature, box_internal_height, box_internal_width);
    getmaxyx(box_input_devices, box_input_height, box_input_width);
    getmaxyx(box_output_devices, box_output_height, box_output_width);

    while (not end_program)
    {
        mvwprintw(box_ground_temperature, 2, (box_external_width / 2) - 6, control.get_temperature_info("Térreo").c_str());
        mvwprintw(box_first_temperature, 2, (box_internal_width / 2) - 6, control.get_temperature_info("1º Andar").c_str());
        mvwprintw(box_people_counter, 2, (box_reference_width / 2) - 2, control.get_people_amount().c_str());
        mvwprintw(box_alarm_mode, 2, (box_alarm_mode_width / 2) - 4, control.get_alarm_mode().c_str());

        auto count_input = 2;
        auto count_output = 2;
        for (const auto &sensor : control.get_sensors_info())
        {
            if (sensor.second.mode == "output")
            {
                mvwprintw(box_output_devices, count_output, 2, "                                                       ");
                mvwprintw(box_output_devices, count_output, 2, sensor.second.tag.c_str());
                mvwprintw(box_output_devices, count_output++, box_output_width - 4, (sensor.second.state ? "ON " : "OFF"));
            }
            else
            {
                mvwprintw(box_input_devices, count_input, 2, "                                                       ");
                mvwprintw(box_input_devices, count_input, 2, sensor.second.tag.c_str());
                mvwprintw(box_input_devices, count_input++, box_input_width - 4, (sensor.second.state ? "ON " : "OFF"));
            }
        }

        wrefresh(box_ground_temperature);
        wrefresh(box_first_temperature);
        wrefresh(box_people_counter);
        wrefresh(box_alarm_mode);

        wrefresh(box_output_devices);
        wrefresh(box_input_devices);

        sleep(1);
    }
}