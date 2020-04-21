#ifndef COMMON_UTILS_TCPSERVER_H
#define COMMON_UTILS_TCPSERVER_H_

#include "../../base/Base.h"
#include <tacopie/tacopie>

class TCPServer 
{
private:
    tacopie::tcp_server server_;
    IPv4* ip_;
    std::condition_variable cv;
public:
    TCPServer(IPv4 *ip) 
    {
        ip_ = ip;
    }

    void signint_handler(int) 
    {
        cv.notify_all();
    }

    void start(std::string &ip) 
    {
        if (server_.is_running())
        {
            server_.stop();
        }

        std::string tmp;
        try 
        {
            server_.start(ip_->getIP(), ip_->getPort(), [&](const std::shared_ptr<tacopie::tcp_client>& client) -> bool {
                tmp = client->get_host();
                std::cout << std::endl;
                std::cout << "---Received new request from: "<< tmp << ":" << client->get_port() << std::endl;
                std::cout << std::endl;
                cv.notify_all();
                return true;
            });
        }
        catch (const std::exception& e)
        {
            std::cout << "###TCP Server error: " << e.what() << std::endl;
            return;
        }

        std::mutex mtx;
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
        ip = tmp;
    }

    void stop()
    {
        server_.stop();
    }
};

#endif