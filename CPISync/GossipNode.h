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
//delete db to prevent memory leaks as well as to pass leveldb status.ok()
string name, key;
DB* db = nullptr;
~GossipNode()
{
delete db;
}
void init(string nodeName, string key, string initialElems, GenSync& genSync);
void connect(GenSync& genSync);
void listen(GenSync& genSync);
};
}
