#ifndef COMMON_UTILS_TCPSERVER_H
#define COMMON_UTILS_TCPSERVER_H_

#include "../base/Base.h"

class TCPServer 
{
private:
    tacopie::tcp_server server_;
    IPv4* ip_;
    std::condition_variable cv;
public:
    TCPServer(IPv4 *ip) {
        ip_ = ip;
    }

    void signint_handler(int) {
        cv.notify_all();
    }

    void start(std::string &ip) {
        std::string tmp;
        server_.start(ip_->getIP(), ip_->getPort(), [&](const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
            tmp = client->get_host();
            cv.notify_all();
            return true;
        });

        std::mutex mtx;
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        ip = tmp;
    }
};

#endif