#include <leveldb/db.h>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std;
using namespace leveldb;
using namespace std;

int main() {
    DB *db = nullptr;
    Options op;
    op.create_if_missing = true;
    Status status = DB::Open(op, "testdb", &db);
    assert(status.ok());

    auto currentTime = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(currentTime);
    string timeStr = ctime(&time);
    string log = timeStr + " write" + " 5"; 
    // log format: "logID","timestamp, action, value" 
    db->Put(WriteOptions(), "001", log);
    string s;
    db->Get(ReadOptions(), "001", &s);
    cout << s << endl;
    delete db;
    return 0;
}
