/*
*   Useful static methods and Node class for using CPISync with leveldb.
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
//Node class
class Node
{
public:
//delete db to prevent memory leaks as well as to pass leveldb status.ok()
string name, key;
DB* db = nullptr;
~Node()
{
delete db;
}
void init(string nodeName, string key, string initialElems, GenSync& genSync);
void connect(GenSync& genSync);
void listen(GenSync& genSync);
};
//static methods
class Gossip
{
public:
    //returns all char elements of 'genSync' in the form of a string and prints the elements
    static string getElems(GenSync &genSync);
    //saves each char of 'elems' in 'genSync'
    static inline void putElems (GenSync &genSync, string elems)
    {
        for (int i = 0; i < elems.length(); ++i)
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
    //prints out all nodes' values corresponding to key 'key'
    static void getNodeValues(vector<string> nodes, string key);
};
}
