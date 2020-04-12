#ifndef COMMON_UTILS_IPV4_H_
#define COMMON_UTILS_IPV4_H_

#include "../base/Base.h"

class IPv4 {
public:
    std::string ip_;
    int port_;
    IPv4(std::string ip, int port) {
        ip_ = ip;
        port_ = port;
    }
};
#endif
