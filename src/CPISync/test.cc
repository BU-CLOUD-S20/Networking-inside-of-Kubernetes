#include "../common/base/Base.h"

using namespace std;

bool server;
const int TCP_PORT = 8002;

void listenTCP(string ip, int port)
{

        IPv4 *IP = new IPv4(ip, port);
        TCPServer *server = new TCPServer(IP);
        std::string res;
        server->start(res); 
        //currentNode->addNeighbor(res);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
}
int main()  {
    //std::thread tcp_thread(listenTCP, "172.128.1.1", TCP_PORT);

    IPv4 *IP = new IPv4("172.28.1.1", TCP_PORT);
    TCPServer *server = new TCPServer(IP);
    std::string res;
    server->start(res); 
    //tcp_thread.join();
    return 0;
}