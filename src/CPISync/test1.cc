#include "../common/base/Base.h"

using namespace std;

const int TCP_PORT = 8002;

void call() {
    std::vector<IPv4*> v;
    v.push_back(new IPv4("172.28.1.1", TCP_PORT));
    TCPClient *client = new TCPClient(v);
    cout << "ready to call client.start" << endl;
    client->start();
}
int main()  {
    std::thread t1(call);
    t1.join();
    return 0;
}