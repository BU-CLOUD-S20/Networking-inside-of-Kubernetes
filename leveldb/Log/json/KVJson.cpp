// warm remind: gcc KVJson.cpp -lstdc++ -o KVJson
// difficult with GCC: https://en.wikibooks.org/wiki/JsonCpp

// Use another single file

#include <iostream>
#include <chrono>
#include <ctime> 
#include <typeinfo>       // operator typeid
#include <string>     // to use std::string, std::to_string() and "+" operator acting on strings 

#include "json.hpp"
//#include <nlohmann/json.hpp>


// logID will increase by itself for each new log come in
int logID = 1;

/*
void newKeyValLog(std::string key, std::string val) {
  // in the furture, we can add a verification scheme for key from client. (maybe)  

  // prepare a final string
  std::string newLog;

  // get current time
  auto end = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  
  // cout << std::ctime(&end_time);
  // cout << typeid(std::ctime(&end_time)).name(); // char * _ptr64

  // logID is key for new log
  newLog += std::to_string(logID);
  newLog += " : ";
  newLog += val;
  newLog += " | ";
  // add in time stamp
  newLog += std::string(std::ctime(&end_time));
  
  std::cout << newLog;

}
*/


void newKeyValJson(std::string key, std::string val) {
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
  j[std::string(std::ctime(&end_time))] = { val, std::to_string(logID) };
  
  std::cout << j << std::endl;
}

// Basic test, only 2 calls
// We can add for loop for more tests
int main() {
  int numofLog = 5;
  
  for(int i = 0; i < numofLog; i++) {
    std::string keyString = "key of Client" + std::to_string(i);
    std::string valString = "value from Client" + std::to_string(i);
    // newKeyValLog(keyString, valString);
    newKeyValJson(keyString, valString);
    // everytime call newKEyValLog, increase logID
    logID += 1;
  }

  return 0;
}
