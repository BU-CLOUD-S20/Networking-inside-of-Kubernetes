// Receive key and value 2 strings from client
// Output a string in format: "key : value | timestamp"

#include <iostream>
#include <chrono>
#include <ctime> 
#include <typeinfo>       // operator typeid
#include <string>     // to use std::string, std::to_string() and "+" operator acting on strings 


using namespace std;

// logID will increase by itself for each new log come in
int logID = 1;

void newKeyValLog(string key, string val) {
	// in the furture, we can add a verification scheme for key from client. (maybe)  

	// prepare a final string
	string newLog;

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
	
	cout << newLog;

}

// Basic test, only 2 calls
// We can add for loop for more tests
int main() {
	newKeyValLog("keyClient", "valClient");

	// everytime call newKEyValLog, increase logID
	logID += 1;

	newKeyValLog("keyClient2", "valClient2");
	// everytime call newKEyValLog, increase logID
	logID += 1;

	return 0;
}