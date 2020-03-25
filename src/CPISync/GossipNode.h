/*
*   Node class, where each node has a leveldb store and can communicate through CPISync.
*   Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#pragma once
#include <iostream>
#include <string>
#include <leveldb/db.h>
#include <CPISync/Syncs/GenSync.h>
#include <CPISync/Data/DataObject.h>

using namespace NTL;
using namespace leveldb;
using std::cout;
using std::endl;
using std::string;

namespace niok
{
class GossipNode
{
public:
//Metadata
    string name;
    hash<string> strHash;
    vector <string> log;
    vector <string> neighbors;
    DB* hashDefs = nullptr; //use a map in final version
    // map<string, string> hashDefs;
//map <string, string> hashDefs;
//Data
    DB* db = nullptr;
//initialize node using a hash function
    GossipNode(hash<string> hashFunc)
    {
        strHash = hashFunc;
        //  hashDefs = getDB("hashDefs");
    }
//delete db to prevent memory leaks as well as to pass leveldb status.ok()
    ~GossipNode()
    {
        delete db;
    }
    //main methods
    void init(string nodeName, vector<string> initialElems, GenSync& genSync);
    void connect(GenSync& genSync);
    void listen(GenSync& genSync);

private:
int EOL = 0;


//helper functions
    DB* getDB(string name)
    {
        retry:
        DB *db = nullptr;
        Options op;
        op.create_if_missing = true;
        Status status = DB::Open(op, name, &db);
        if (!status.ok())
        {
            usleep (50000);
            goto retry;
        }
        return db;
    }
    //gets diff from last sync
    void getDiff(GenSync &genSync);
    //gets all elements
    void getElems(GenSync &genSync);
    //prints all elements in log
    void getLog();
};
}
