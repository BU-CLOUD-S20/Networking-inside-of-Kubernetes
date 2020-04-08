#include "../common/base/Base.h"
#include "json.hpp"
#include "JsonParsing.cc"

using std::string;
using namespace niok;
//namespace niok{

int main() {
  // name of the logging file
  string logFile = "exampleLog.json";

  // add 5 keys and values to the log
  for (int i = 0; i < 5; i++) {
    string key = "Timestamp" + std::to_string(i);
    string value = "Value" + std::to_string(i);
    log::addLogToFile(logFile, key, value);
  }

  std::cout << "Printing File Contents:\n";
  log::printFileContents(logFile);

  std::cout << "printing value from key 0 " << log::getValueFromKey(logFile, "Timestamp0") << std::endl;
  std::cout << "printing value from key 1 " << log::getValueFromKey(logFile, "Timestamp1") << std::endl;
  std::cout << "printing value from key 2 " << log::getValueFromKey(logFile, "Timestamp2") << std::endl;
  std::cout << "printing value from key 3 " << log::getValueFromKey(logFile, "Timestamp3") << std::endl;
  std::cout << "printing value from key 4 " << log::getValueFromKey(logFile, "Timestamp4") << std::endl;

  return 0;
}
//}