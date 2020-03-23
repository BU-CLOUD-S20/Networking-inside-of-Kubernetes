/*
*   Node class, where each node has a leveldb store and can communicate through CPISync.
*   Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#pragma once
#include "GossipUtilities.h"
#include "../kvstore/KVEngine.h"
#include "../kvstore/LevelEngine.h"

namespace niok {
namespace cpisync {

class GossipNode
{
public:
    //Metadata
    string name_;
    //Constructors
    GossipNode(){}
    GossipNode(string nodeName, int spaceId, string path) {
        name_ = nodeName;
        rootPath_ = path;
        db_ = new kvstore::LevelEngine(spaceId, rootPath_);
    }
    //delete db to prevent memory leaks as well as to pass leveldb status.ok()
    ~GossipNode()
    {
        delete db_;
    }
    void init(string nodeName, vector<string> initialElems, GenSync& genSync);
    void connect(GenSync& genSync);
    void listen(GenSync& genSync);

    bool get(const std::string& key, std::string* value);

    bool put(std::string key, std::string value);

private:
    vector <string> log_;
    vector <string> neighbors_;
    kvstore::LevelEngine* db_;
    string rootPath_;
}; 

} //namespace cpisync
} //namespace niok
