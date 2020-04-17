#include "TCPClient.h"
#include "TCPServer.h"

#include "../base/Base.h"

int main(int argc, char *argv[])
{
    const int NODE = std::stoi(argv[1]);

    IPv4 *ip = new IPv4("172.28.1.1", 8001);
    if (NODE == 1) {
        TCPServer *server = new TCPServer(ip);
        std::string res;
        server->start(res); 
        std::cout << res << std::endl;
    } else {
        std::vector<IPv4*> v;
        v.push_back(ip);
        TCPClient *client = new TCPClient(v);
        client->start();
    }
    return 0;
}