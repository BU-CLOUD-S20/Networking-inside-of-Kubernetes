/*
*   A class of useful static methods for using CPISync with leveldb.
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
    static inline string getElems(GenSync &genSync)
    {
        string str;
        for (string dop : genSync.dumpElements())
        {
            //print char data in proper format and save to 'str'
            char data = (char)(((int)dop[0])+2);
            cout<< data << " ";
            str.push_back(data);
        }
        cout << endl;
        return str;
    }
    static inline void putElems (GenSync &genSync, string elems)
    {
        for (int i = 0; i < elems.length(); ++i)
        {
             genSync.addElem(make_shared<DataObject>(elems[i]));
        }
    }
    static inline DB* getDB(string name)
    {
        DB *db = nullptr;
        Options op;
        op.create_if_missing = true;
        Status status = DB::Open(op, name, &db);
        assert(status.ok());
        return db;
    }
    static inline void getNodeValues(vector<string> nodes, string key)
    {
        string s;
        cout << "Current value of key '" << key << "' for all nodes:" <<endl;
        for (int i =0; i<nodes.size(); ++i)
        {
            DB *db = nullptr;
            db = getDB(nodes[i]);
            db->Get(ReadOptions(), key, &s);
            cout<< nodes[i] << ": " << s << endl;
            delete db;
        }
    }
};
}
