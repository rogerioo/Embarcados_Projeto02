#include <socket.hpp>

using namespace std;

int main(int argc, char const *argv[])
{
    Socket socket("192.168.0.22", 8080);

    socket.send_data("{\"option\": \"toggle\",\"gpio\": 7,\"signal\": 1}");

    return 0;
}
