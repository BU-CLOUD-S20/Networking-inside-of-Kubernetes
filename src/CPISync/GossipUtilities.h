/*
*   Useful static methods for using CPISync with leveldb.
*   Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#pragma once
#include <iostream>
#include <string>
#include <leveldb/db.h>
#include <CPISync/Syncs/GenSync.h>
#include <CPISync/Data/DataObject.h>

using namespace leveldb;
using std::cout;
using std::endl;
using std::string;

namespace niok
{
class Gossip
{
public:
    //returns all strings in'genSync' and prints them
    static vector<string> getElems(GenSync &genSync);
    //saves each string of 'elems' in 'genSync'
    static inline void putElems (vector<string> elems, GenSync &genSync)
    {
        for (int i = 0; i < elems.size(); ++i)
        {
             genSync.addElem(make_shared<DataObject>(elems[i]));
        }
    }
    //returns pointer to a DB given the string 'name'
    static inline DB* getDB(string name)
    {
        DB *db = nullptr;
        Options op;
        op.create_if_missing = true;
        Status status = DB::Open(op, name, &db);
        assert(status.ok());
        return db;
    }
};
}
