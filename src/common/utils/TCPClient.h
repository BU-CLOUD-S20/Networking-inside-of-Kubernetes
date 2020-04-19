#ifndef COMMON_UTILS_TCPCLIENT_H
#define COMMON_UTILS_TCPCLIENT_H_

#include "../base/Base.h"
#include <tacopie/tacopie>

class TCPClient 
{
private:
    tacopie::tcp_client client_;
    std::vector<IPv4*> ips_;
    std::condition_variable cv;
public:
    TCPClient(std::vector<IPv4*> ips) {
        ips_ = ips;
    }

    void signint_handler(int) {
        cv.notify_all();
    }

    void start() {
        for (auto ip : ips_) {
            std::string addr = ip->getIP();
            int port = ip->getPort();
            client_.connect(addr, port);
        }
    }
};
#endif