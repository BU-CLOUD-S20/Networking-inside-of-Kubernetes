#include "GossipNode.h"

namespace niok
{
namespace cpisync
{

GossipNode::GossipNode(string nodeName, int spaceId, string path,
               hash<string> hashFunc, vector<string> initialElems)
{
    name_ = nodeName;
    rootPath_ = path;
    db_ = new kvstore::LevelEngine(spaceId, rootPath_);
    strHash = hashFunc;
    //fill log and add defs
    for (int i = 0; i < initialElems.size(); ++i)
    {
        log_.push_back(initialElems[i]);
        hashDefs[to_string(strHash(initialElems[i]))] = initialElems[i];
    }
    //cout << "All logs of current node:" << endl << endl;
    for (int i = 0; i<log_.size(); ++i)
    {
        //cout << " " + log_.at(i);
    }
    //cout <<endl << endl;
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
            cout << "ResultCode: " << resultCode << endl;
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
            cout << "ResultCode: " << resultCode << endl;
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
        cout << "ResultCode: " << resultCode << endl;
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

    sync(HOST, NUM_CHAR, true);
    
    return commit(logEntry);
}

bool GossipNode::remove(std::string key)
{
    string logEntry = keyValueToLog(key, "", "R");
    log_.push_back(logEntry);
    hashDefs[to_string(strHash(logEntry))] = logEntry;

    sync(HOST, NUM_CHAR, true);
    
    return commit(logEntry);
}

void GossipNode::sync(string host, int NUM_CHAR, bool server)
{
    string res;
    //sync hashes
    // cout << "New hashed logs recieved:" << endl <<endl;
    if (server)
    {
        res = exec("./sync " + host+ " s " + to_string(NUM_CHAR) + getNewHashedLogs());
        // cout<<res <<endl <<endl;
    }
    else
    {
        res = exec("./sync " + host+ " c " + to_string(NUM_CHAR) + getNewHashedLogs());
        // cout<<res <<endl <<endl;
    }
    //request hash definition
    //cout << "Hash definitions requested:" <<endl <<endl;
    if (server)
    {
        res = exec("./sync " + host+ " s " + to_string(NUM_CHAR) + res);
        // cout<<res <<endl << endl;
    }
    else
    {
        res = exec("./sync " + host+ " c " + to_string(NUM_CHAR) + res);
        //cout<<res <<endl <<endl;
    }
    //send hash definition
    string defs;
    char* tok = strtok(getCharsFromString(res), " ");
    while (tok!= NULL)
    {
        defs+= (" "+hashDefs[tok]);
        tok = strtok (NULL, " ");
    }
    // cout << "New logs from sync:" << endl <<endl;
    if (server)
    {
        res = exec("./sync " + host+ " s " + to_string(NUM_CHAR) + defs);
    }
    else
    {
        res = exec("./sync " + host+ " c " + to_string(NUM_CHAR) + defs);
    }

    tok = strtok(getCharsFromString(res), " ");
    while (tok!= NULL)
    {
        log_.push_back(tok);
        tok = strtok (NULL, " ");
    }
    EOL = log_.size();
    // cout << endl;
}

string GossipNode::keyValueToLog(string key, string value, string op) {
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

vector<string> GossipNode::logToKeyValue(string log) {
    vector<string> res;
    istringstream is(log);
    string tmp;
    while (getline(is, tmp, '+')) {
        res.push_back(tmp);
    }
    is.clear();
    return res;
}

void GossipNode::processLogEntry()
{
    int i = EOC;
    for (i; i < log_.size(); i++) {
        string logEntry = log_[i];
        if (!commit(logEntry)) 
        {
            cout << "this LogEntry commited failed : ";
            cout << logEntry << endl;
        }
    }
    EOC = i;
}
string GossipNode::getNewHashedLogs()
{
    string res;
    for (int  i = log_.size()-1; i > EOL -1; --i)
    {
        res+= " " + to_string(strHash(log_.at(i)));
    }
    return res;
}

} // cpisync
} // namespace niok
