// warm remind: gcc KVJson.cpp -lstdc++ -o KVJson
// difficult with GCC: https://en.wikibooks.org/wiki/JsonCpp

// Use another single file

#include <iostream>
#include <chrono>
#include <ctime> 
#include <typeinfo>       // operator typeid
#include <string>     // to use std::string, std::to_string() and "+" operator acting on strings 
#include <fstream>
#include "json.hpp"
//#include <nlohmann/json.hpp>

using namespace std;


// logID will increase by itself for each new log come in
int logID = 1;

int readFile(string file, string key) {
  ifstream log(file);
  nlohmann::json j;
  log >> j;

  cout << j[key];

  return 1;
}

int outputToFile(nlohmann::json j) {
  ofstream log;
  log.open ("exampleLog.json", ios_base::app);
  log << j << "\n";
  log.close();
  return 1;
}

int printFile() {
  string line;
  ifstream log ("exampleLog.json");
  nlohmann::json j;

  if (log.is_open()) {
    while (getline (log, line)) {
      cout << line << "\n";
    }
    log.close();
    return 1;
  }

  return 0;
}

nlohmann::json newKeyValJson(string key, string val) {
  // prepare a final string
  std::string newLog;

  // get current time
  auto end = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  // create an empty structure (null)
  nlohmann::json j;

  // add another object (using an initializer list of pairs)
  // i.e. j["object"] = { {"currency", "USD"}, {"value", 42.99} };
  // j[std::to_string(logID)] = { val, std::string(std::ctime(&end_time)) };
  // key is time of log; val is val and logID
  //j[string(ctime(&end_time))] = { val, to_string(logID) };
  key = string(ctime(&end_time))

  j[key] = { val, to_string(logID) };

  std::cout << j << std::endl;
  return j;
}

// Basic test, only 2 calls
// We can add for loop for more tests
int main() {
  int numofLog = 5;
  
  for(int i = 0; i < numofLog; i++) {
    string keyString = "key of Client" + std::to_string(i);
    string valString = "value from Client" + std::to_string(i);
    //outputToFile(newKeyValJson(keyString, valString));
    logID++;
  }
  printFile();
  cout << "\n";
  readFile("exampleLog.json", "Thu Mar 26 20:44:27 2020\n");

  /*
  nlohmann::json test;
  test["Hello"] = "World";
  test["pepper"] = "oni";

  cout << test << '\n';
  cout << test["pepper"];
  */

  return 0;
}
