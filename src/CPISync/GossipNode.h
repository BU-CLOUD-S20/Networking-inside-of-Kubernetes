/*
*   Node class, where each node has a leveldb store and can communicate through CPISync.
*   See niok.cc for example of usage.
*   Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#ifndef KVSTORE_GOSSIPNODE_H_
#define KVSTORE_GOSSIPNODE_H_

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

    //Constructor
    GossipNode(hash<string> hashFunc, int numCharHash, int numCharEntry,
               vector<string> initialEntries);
    //delete db to prevent memory leaks as well as to pass leveldb status.ok()
    ~GossipNode()
    {
        delete db_;
    }

    //====Operations=====================
    bool get(const std::string& key, std::string* value);
    bool put(std::string key, std::string value);
    bool remove(std::string key);
    void joinCluster(std::vector<string> &ips);
    void addNeighbor(string ip);
    void showNeighbors();
    //====TCP Connection=================
    void static listenTCP();
    std::vector<string> connectTCP(std::vector<string> &ips);
    //====Commit Operations==============
    void processLogEntry();
    //=====Sync Logs=====================
    void sync(string host, bool server, int times = 1);
    //=======Helper Functions============
    vector<string> logToKeyValue(string log);
    string keyValueToLog(string key, string value, string op);
    //used in processLogEntry()
    bool commit(std::string log);
    //modified version of: https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
    string exec(string strCmd)
    {
        char* cmd= getCharsFromString(strCmd);
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
    //convert a string 'str' to char* and append with 'append'
    char* getCharsFromString(string str)
    {
        char* res=new char[str.length()+1];
        str.copy(res, str.size());
        res[str.length()] = '\0'; //end string with null
        return res;
    }

    string name_;
    string ip_;
    string rootPath_;
private:
    //log
    vector <string> log_; //vector of log entries
    unordered_set<string> neighbors_;
    int EOL = 0; //end of log, any higher indicies in log_ are not synced yet
    int EOC = 0; //end of commited log entires, any higher or equal indecies in log_ are not commited to local
    //Hash Sync
    const int NUM_CHAR_ENTRY; //max num char in log entries
    const int NUM_CHAR_HASH; //num char in each hash (i.e. use 20 for 64 bit hash)
    hash<string> strHash; //hash function to use
    map <string, string> hashDefs; //definitions of hashed log entries
    //KV
    kvstore::LevelEngine* db_;
    const int SPACE_ID = 0;
    const int CPI_PORT = 8001;
    const int TCP_PORT = 8002;

};

} //namespace cpisync
} //namespace niok
#endif
