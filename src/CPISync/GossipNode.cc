#include "GossipNode.h"

namespace niok
{
namespace cpisync
{

GossipNode::GossipNode(hash<string> hashFunc, int numCharHash, int numCharEntry, //hashing
                       vector<string> initialEntries) //initial log entries
    :NUM_CHAR_HASH(numCharHash),NUM_CHAR_ENTRY(numCharEntry) //const variables
{
    // initialize node's IP and Name
    NetworkUtils *network = new NetworkUtils();
    std::string hostName;
    std::string Ip;
    bool ret = network->GetHostInfo(hostName, Ip);
    if (true == ret)
    {
        ip_ = Ip;
        name_ = hostName;
    }
    else
    {
        std::cout << "###Initialize Failed" << std::endl;
        std::cout << "###Getting host name and ip failed" << std::endl;
        exit(1);
    }
    // init db path
    rootPath_ = "/tmp/" + name_;
    // init db
    db_ = new kvstore::LevelEngine(SPACE_ID, rootPath_);

    strHash = hashFunc;
    //initialize log_ and hashDefs
    for (int i = 0; i < initialEntries.size(); ++i)
    {
        log_.push_back(initialEntries[i]);
        hashDefs[to_string(strHash(initialEntries[i]))] = initialEntries[i];
    }
    processLogEntry();
}

bool GossipNode::commit(std::string log)
{
    vector<string> separated = logToKeyValue(log);
    if (separated[0] == "P")
    {
        string key = separated[2], value = separated[3];
        auto resultCode = db_->put(key, value);
        if (resultCode != kvstore::ResultCode::SUCCEEDED)
        {
            cout << "###ResultCode: " << resultCode << endl;
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (separated[0] == "R")
    {
        string key = separated[2];
        auto resultCode = db_->remove(key);
        if (resultCode != kvstore::ResultCode::SUCCEEDED)
        {
            cout << "###ResultCode: " << resultCode << endl;
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool GossipNode::get(const std::string& key, std::string* value)
{
    auto resultCode = db_->get(key, value);
    if (resultCode != kvstore::ResultCode::SUCCEEDED)
    {
        cout << "###ResultCode: " << resultCode << endl;
        return false;
    }
    else
    {
        return true;
    }
}

bool GossipNode::put(std::string key, std::string value)
{
    //fill log and add defs
    string logEntry = keyValueToLog(key, value, "P");
    log_.push_back(logEntry);
    hashDefs[to_string(strHash(logEntry))] = logEntry;
    return true;
}

bool GossipNode::remove(std::string key)
{
    string logEntry = keyValueToLog(key, "", "R");
    log_.push_back(logEntry);
    hashDefs[to_string(strHash(logEntry))] = logEntry;
    return true;
}

void GossipNode::sync(string host, bool server)
{
    int logSize = log_.size();
    string hashes;
    for (int  i = EOL[host]; i <logSize; ++i)
    {
        hashes+= " " + to_string(strHash(log_.at(i)));
    }
    string cmd = (server)? "./sync " + host + " s "
                 :"./sync " + host + " c ";
    //sync hash
    hashes = exec(cmd + to_string(NUM_CHAR_HASH) + hashes);
    //save vector of unknownHashes for later
    vector<string> unknownHashes;
    char* tok = strtok(getCharsFromString(hashes), " ");
    while (tok!= NULL)
    {
        unknownHashes.push_back(tok);
        tok = strtok (NULL, " ");
    }
    //request definitions
    hashes = exec(cmd + to_string(NUM_CHAR_HASH) + hashes);
    //send definitions
    string defs;
    tok = strtok(getCharsFromString(hashes), " ");
    while (tok!= NULL)
    {
        defs+= (" "+hashDefs[tok]);
        tok = strtok (NULL, " ");
    }
    defs = exec(cmd + to_string(NUM_CHAR_ENTRY) + defs);
    //update hashDefs and log_
    tok = strtok(getCharsFromString(defs), " ");
    int i = 0;
    while (tok!= NULL)
    {
        hashDefs[unknownHashes.at(i)] = tok;
        log_.push_back(tok);
        tok = strtok (NULL, " ");
        ++i;
    }
    //update EOL
    EOL[host] = log_.size();
}

void GossipNode::sync(unordered_set<string> hosts, bool server)
{
//sync for 'times' times
    int logSize = log_.size();
    for (string host : hosts)
    {
        string hashes;
        for (int  i = EOL[host]; i <logSize; ++i)
        {
            hashes+= " " + to_string(strHash(log_.at(i)));
        }
        string cmd = (server)? "./sync " + host + " s "
                     :"./sync " + host + " c ";
        //sync hash
        hashes = exec(cmd + to_string(NUM_CHAR_HASH) + hashes);
        //save vector of unknownHashes for later
        vector<string> unknownHashes;
        char* tok = strtok(getCharsFromString(hashes), " ");
        while (tok!= NULL)
        {
            unknownHashes.push_back(tok);
            tok = strtok (NULL, " ");
        }
        //request definitions
        hashes = exec(cmd + to_string(NUM_CHAR_HASH) + hashes);
        //send definitions
        string defs;
        tok = strtok(getCharsFromString(hashes), " ");
        while (tok!= NULL)
        {
            defs+= (" "+hashDefs[tok]);
            tok = strtok (NULL, " ");
        }
        defs = exec(cmd + to_string(NUM_CHAR_ENTRY) + defs);
        //update hashDefs and log_
        tok = strtok(getCharsFromString(defs), " ");
        int i = 0;
        while (tok!= NULL)
        {
            hashDefs[unknownHashes.at(i)] = tok;
            log_.push_back(tok);
            tok = strtok (NULL, " ");
            ++i;
        }
        //kill server sync if needed
        if (!server)
            system("./tmp > /dev/null 2>&1");
         //update EOL
         EOL[host] = log_.size();
    }
}

string GossipNode::keyValueToLog(string key, string value, string op)
{
    Time t;
    string res;
    if (op == "P")
    {
        res = op + "+" + t.getCurrentTime() + "+" + key + "+" + value;
    }
    else if (op == "R")
    {
        res = op + "+" + t.getCurrentTime() + "+" + key;
    }
    return res;
}

vector<string> GossipNode::logToKeyValue(string log)
{
    vector<string> res;
    istringstream is(log);
    string tmp;
    while (getline(is, tmp, '+'))
    {
        res.push_back(tmp);
    }
    is.clear();
    return res;
}

void GossipNode::processLogEntry()
{
    const int logSize = log_.size();
    for (int i=EOC; i < logSize; ++i)
    {
        if (!commit(log_[i]))
        {
            cout << "###this LogEntry commited failed : ";
            cout << log_[i] << endl;
        }
    }
    EOC = logSize;
}

void GossipNode::listenTCP()
{

}

std::vector<std::string> GossipNode::connectTCP(std::vector<string> &ips)
{
    std::vector<IPv4*> v;
    for (auto ip : ips)
    {
        v.push_back(new IPv4(ip, TCP_PORT));
    }
    TCPClient *client = new TCPClient(v);
    auto res = client->start();
    return res;
}

void GossipNode::joinCluster(std::vector<string> &ips)
{
    auto res = connectTCP(ips);
    for (auto ip : res)
    {
        neighbors_.insert(ip);
    }
}

void GossipNode::addNeighbor(string ip)
{
    neighbors_.insert(ip);
    //init each neighbor EOL
    EOL[ip] = 0;
}

void GossipNode::showNeighbors()
{
    if (neighbors_.empty())
    {
        cout << "---This node has no neighbor" << endl;
        cout << endl;
        return;
    }
    cout << "---Neighbor(s):" << endl;
    for (auto neighbor : neighbors_)
    {
        cout << neighbor << endl;
    }
    cout << endl;
}
} // cpisync
} // namespace niok

