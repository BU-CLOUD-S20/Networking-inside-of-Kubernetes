#include "../common/base/Base.h"
#include "json.hpp"

using json = nlohmann::json;
using std::string;
namespace niok {
namespace log {
/*
    Still need to implement accurate timestamping for keys
*/

// returns a value from a given key 
string getValueFromKey(string file, string key) {
    // read in the file
std::ifstream test(file);
    std::stringstream buffer;
    buffer << test.rdbuf();

    // make a json object out of the file contents
    // and return the value for the given key
    json j;
    j = json::parse(buffer.str());

    return j[key].dump();
}

// adds a new log with key and value to a file
int addLogToFile(string file, string key, string value) {

    // read in the file
    std::ifstream test(file);
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
    log.open(file);
    log << j;
    log.close();

    return 1;
}

// prints the contents of a file 
int printFileContents(string file) {
    string line;
    std::ifstream log (file);
    json j;

    if (log.is_open()) {
      while (getline (log, line)) {
        std::cout << line << "\n";
      }
      log.close();
      return 1;
    }

    return 0;
}

// creates a new json object with given key and value
json createNewJson(string key, string val) {

    string newLog;
    json j;
    
    j[key] = { val };

    return j;
}
} // namespace log
} // namespace niok
