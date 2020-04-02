#include <iostream>
#include <chrono>
#include <ctime> 
#include <typeinfo>       
#include <string>      
#include <fstream>
#include "json.hpp"
#include "jsonParsing.cpp"

using namespace std;

int main() {
  // name of the logging file
  string logFile = "exampleLog.json";

  // add 5 keys and values to the log
  for (int i = 0; i < 5; i++) {
    string key = "Timestamp" + to_string(i);
    string value = "Value" + to_string(i);
    addLogToFile(logFile, key, value);
  }

  cout << "Printing File Contents:\n";
  printFileContents(logFile);

  cout << "printing value from key 0 " << getValueFromKey(logFile, "Timestamp0") << endl;
  cout << "printing value from key 1 " << getValueFromKey(logFile, "Timestamp1") << endl;
  cout << "printing value from key 2 " << getValueFromKey(logFile, "Timestamp2") << endl;
  cout << "printing value from key 3 " << getValueFromKey(logFile, "Timestamp3") << endl;
  cout << "printing value from key 4 " << getValueFromKey(logFile, "Timestamp4") << endl;

  return 0;
}
