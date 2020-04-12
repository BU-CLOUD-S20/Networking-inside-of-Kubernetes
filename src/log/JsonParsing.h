#ifndef LOG_JSONPARSING_H_
#define LOG_JSONPARSING_H_

#include "../common/base/Base.h"
#include "json.hpp"

using json = nlohmann::json;

namespace niok {
namespace log {

class JsonParsing 
{
public:
    JsonParsing(std::string name);

    ~JsonParsing(){

    };

    std::string getValueFromKey(std::string key);

    void addLogToFile(std::string key, std::string value);

    void addLogToFileTimestamp(std::string value);

    void printFileContents();

    json createNewJson(std::string key, std::string val);

private:
    std::string name_;
};

} // namespace log
} // namespace niok

#endif
