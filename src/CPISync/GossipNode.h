/*
*   Node class, where each node has a leveldb store and can communicate through CPISync.
*   Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#pragma once
#include "GossipUtilities.h"

namespace niok
{
class GossipNode
{
public:
//Metadata
string name;
vector <string> log;
vector <string> neighbors;
//Data
DB* db = nullptr;
//delete db to prevent memory leaks as well as to pass leveldb status.ok()
~GossipNode()
{
delete db;
}
void init(string nodeName, vector<string> initialElems, GenSync& genSync);
void connect(GenSync& genSync);
void listen(GenSync& genSync);
};
}
