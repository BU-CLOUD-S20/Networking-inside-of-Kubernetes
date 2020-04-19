#ifndef COMMON_UTILS_TCPCLIENT_H
#define COMMON_UTILS_TCPCLIENT_H_

#include "../../base/Base.h"
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

    std::vector<std::string> start() {
        std::vector<std::string> res;
        for (auto ip : ips_) {
            std::string addr = ip->getIP();
            int port = ip->getPort();
            try
            {
                std::cout << "---connecting " << addr << ":" << port << std::endl; 
                client_.connect(addr, port, 500);
                std::cout << "---connected " << addr << ":" << port << std::endl;
                client_.disconnect();
                res.push_back(addr);
            }
            catch(const std::exception& e)
            {
                std::cout << "###TCP Client error: " << e.what() << " when connecting " << addr << ":" << port << std::endl;
            }
        }
        return res;
    }
};
#endif