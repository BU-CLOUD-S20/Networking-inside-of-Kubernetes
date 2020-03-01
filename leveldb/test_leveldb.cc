#include <leveldb/db.h>
#include <cstdio>
#include <iostream>

using namespace std;
using namespace leveldb;

int main() {
    unique_ptr<DB> db();
    Options op;
    op.create_if_missing = true;
    Status status = DB::Open(op, "testdb", &db);
    cout << status.ToString() <<endl;
	assert(status.ok());
    db->Put(WriteOptions(), "001", "Hello world");
    string s;
    db->Get(ReadOptions(), "001", &s);
    cout << s << endl;
    db->Delete(WriteOptions(),"001");
    return 0;
}