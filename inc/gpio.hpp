#include <wiringPi.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <vector>
#include <string>
#include <map>

using json = nlohmann::json;
using namespace std;

class GPIO
{
private:
    static void handle_event_base(pair<string, string> info);
    static void handle_event_4(void);
    static void handle_event_5(void);
    static void handle_event_6(void);
    static void handle_event_7(void);
    static void handle_event_8(void);
    static void handle_event_9(void);
    static void handle_event_10(void);
    static void handle_event_11(void);
    static void handle_event_12(void);
    static void handle_event_13(void);
    static void handle_event_16(void);
    static void handle_event_17(void);
    static void handle_event_18(void);
    static void handle_event_19(void);
    static void handle_event_22(void);
    static void handle_event_23(void);
    static void handle_event_24(void);
    static void handle_event_25(void);
    static void handle_event_26(void);
    static void handle_event_27(void);

public:
    GPIO(string sensors_json_path);
    ~GPIO();
};
