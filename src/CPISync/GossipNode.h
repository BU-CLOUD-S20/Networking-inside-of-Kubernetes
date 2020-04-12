/*
*   Node class, where each node has a leveldb store and can communicate through CPISync.
*   Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#ifndef KVSTORE_GOSSIPNODE_H_
#define KVSTORE_GOSSIPNODE_H_

#pragma once
#include <CPISync/Syncs/GenSync.h>
#include <CPISync/Data/DataObject.h>
#include "../kvstore/KVEngine.h"
#include "../kvstore/LevelEngine.h"
#include "../common/base/Base.h"

using std::cout;
using std::endl;
using std::string;


namespace niok
{
namespace cpisync
{

class GossipNode
{
public:
    //Metadata
    string name_;
    //Constructor
    GossipNode(string nodeName, int spaceId, string path,
               hash<string> hashFunc, vector<string> initialElems);
    //delete db to prevent memory leaks as well as to pass leveldb status.ok()
    ~GossipNode()
    {
        delete db_;
    }
    void sync(string HOST, int NUM_CHAR, bool server);

    vector<string> logToKeyValue(string log);

    string keyValueToLog(string key, string value, string op);

    bool commit(std::string log);

    bool get(const std::string& key, std::string* value);

    bool put(std::string key, std::string value);

    bool remove(std::string key);

    void processLogEntry();
    //HELPER FUNCTIONS
    //gets any new logs added (index>=EOL) separated with spaces, use with exec
    string getNewHashedLogs();
    //modified version of:
    //https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
    string exec(string strCmd)
    {
        char* cmd= new char[strCmd.length()+1];
        for (int i = 0; i<strCmd.length(); ++i)
        {
            cmd[i] = strCmd[i];
        }
        cmd[strCmd.length()] = '\n'; //always make sure to terminate commands
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        delete cmd;
        return result;
    }
    char* getCharsFromString(string str)
    {
        char* res= new char[str.length()+1];
        for (int i = 0; i<str.length(); ++i)
        {
            res[i] = str[i];
        }
        res[str.length()] = ' '; //end string with delimiter
        return res;
    }
private:
    //log
    vector <string> log_;
    vector <string> neighbors_;
    int EOL = 0; //end of log, any higher indicies in log_ are not synced yet
    int EOC = 0; //end of commited log entires, any higher or equal indecies in log_ are not commited to local
    //Hash Sync
    hash<string> strHash;
    map <string, string> hashDefs;
    //KV
    kvstore::LevelEngine* db_;
    string rootPath_;

    const string HOST = "172.28.1.1";
    const int NUM_CHAR = 64;
};

} //namespace cpisync
} //namespace niok
#endif