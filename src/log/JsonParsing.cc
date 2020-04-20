#include "JsonParsing.h"

using json = nlohmann::json;
using std::string;
namespace niok {
namespace log {

JsonParsing::JsonParsing(string name) {
    name_ = name;
}

// removes logs which are 10 minutes older than the current  
void JsonParsing::garbageCollect(string timestamp) {
    // current time minus 10 minutes
    long long int time = std::stoll(timestamp) - 600000;

    // read in the file
    std::ifstream test(name_);
    std::stringstream buffer;
    buffer << test.rdbuf();

    // make a json object out of the file contents
    // and append a new key and value
    json j;
    if (!buffer.str().empty()) {
      j = json::parse(buffer.str());
    }

    // iterate through the logs 
    // only add logs which meet the time requirement
    json cleansed;
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // delete logs older than 10 minutes
        if (std::stoll(it.key()) > time) {
            cleansed[it.key()] = { it.value() };
        }
    }

    // output json object to the file
    std::ofstream log;
    log.open(name_);
    log << cleansed;
    log.close();

}

// returns a value from a given key 
string JsonParsing::getValueFromKey(string key) {
    // read in the file
    std::ifstream test(name_);
    std::stringstream buffer;
    buffer << test.rdbuf();

    // make a json object out of the file contents
    // and return the value for the given key
    json j;
    j = json::parse(buffer.str());

    return j[key].dump();
}

// adds a new log with key and value to a file
void JsonParsing::addLogToFile(string key, string value) {

    // read in the file
    std::ifstream test(name_);
    std::stringstream buffer;
    buffer << test.rdbuf();

    // make a json object out of the file contents
    // and append a new key and value
    json j;
    if (!buffer.str().empty()) {
      j = json::parse(buffer.str());
    }

    j[key] = { value };

    // output the appended json object to the file
    std::ofstream log;
    log.open(name_);
    log << j;
    log.close();
}

// adds a new log with current time as key and a given value
// Format: "Day Hour:Minute:Second:Millisecond"
// returns the assigned timestamp
std::string JsonParsing::addLogToFileTimestamp(string value) {

    // read in the file
    std::ifstream test(name_);
    std::stringstream buffer;
    buffer << test.rdbuf();

    // make a json object out of the file contents
    // and append a new key and value
    json j;
    if (!buffer.str().empty()) {
      j = json::parse(buffer.str());
    }

    // calculate timestamp
    // time since midnight, Janurary 1, 1970 UTC in milliseconds
    unsigned long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
    
    std::stringstream ss;
    ss << timestamp;
    // associate timestamp with given value
    j[ss.str()] = { value };

    // output the appended json object to the file
    std::ofstream log;
    log.open(name_);
    log << j;
    log.close();

    garbageCollect(ss.str());

    return ss.str();
}

// returns the current timestamp
std::string JsonParsing::getTimestamp() {
    // calculate timestamp
    // time since midnight, Janurary 1, 1970 UTC in milliseconds
    unsigned long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::system_clock::now().time_since_epoch()).count();
    
    std::stringstream ss;
    ss << timestamp;
    // associate timestamp with given value
    return ss.str();
}

// prints the contents of a file 
void JsonParsing::printFileContents() {
    string line;
    std::ifstream log (name_);
    json j;

    if (log.is_open()) {
      while (getline (log, line)) {
        std::cout << line << "\n";
      }
      log.close();
    }
}

// creates a new json object with given key and value
json JsonParsing::createNewJson(string key, string val) {

    string newLog;
    json j;
    
    j[key] = { val };

    return j;
}
} // namespace log
} // namespace niok
