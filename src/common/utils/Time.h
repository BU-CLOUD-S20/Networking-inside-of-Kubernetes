#ifndef COMMON_UTILS_TIME_H
#define COMMON_UTILS_TIME_H_

#include "../base/Base.h"

class Time {
public:
    Time() {};
    std::string getCurrentTime() {
        time_t t = time(NULL);
        std::stringstream ss;
        ss << t;
        std::string ts = ss.str();
        return ts;
    }
};
#endif
