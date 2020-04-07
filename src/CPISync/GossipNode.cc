#include "GossipNode.h"

namespace niok
{
namespace cpisync
{

bool GossipNode::get(const std::string& key, std::string* value)
{
    auto resultCode = db_->get(key, value);
    if (resultCode != kvstore::ResultCode::SUCCEEDED)
    {
        cout << resultCode << endl;
        return false;
    }
    else
    {
        return true;
    }
}

bool GossipNode::put(std::string key, std::string value)
{
    auto resultCode = db_->put(key, value);
    if (resultCode != kvstore::ResultCode::SUCCEEDED)
    {
        cout << resultCode << endl;
        return false;
    }
    else
    {
        return true;
    }
}

void GossipNode::sync(string host, int NUM_CHAR, bool server)
{
    string res;
    //sync hashes
    cout << "New hashed logs recieved:" << endl <<endl;
    if (server)
    {
        res = exec("./sync " + host+ " s " + to_string(NUM_CHAR) + getNewHashedLogs());
        cout<<res <<endl <<endl;
    }
    else
    {
        res = exec("./sync " + host+ " c " + to_string(NUM_CHAR) + getNewHashedLogs());
        cout<<res <<endl <<endl;
    }
    //request hash definition
    cout << "Hash definitions requested:" <<endl <<endl;
    if (server)
    {
        res = exec("./sync " + host+ " s " + to_string(NUM_CHAR) + res);
        cout<<res <<endl << endl;
    }
    else
    {
        res = exec("./sync " + host+ " c " + to_string(NUM_CHAR) + res);
        cout<<res <<endl <<endl;
    }
    //send hash definition
    string defs;
    char* tok = strtok(getCharsFromString(res), " ");
    while (tok!= NULL)
    {
        defs+= (" "+hashDefs[tok]);
        tok = strtok (NULL, " ");
    }
         cout << "New logs from sync:" << endl <<endl;
     if (server)
    {
        res = exec("./sync " + host+ " s " + to_string(NUM_CHAR) + defs);
        cout<<res <<endl;
    }
    else
    {
        res = exec("./sync " + host+ " c " + to_string(NUM_CHAR) + defs);
        cout<<res <<endl;
    }
    tok = strtok(getCharsFromString(res), " ");
    while (tok!= NULL)
    {
        log_.push_back(tok);
        tok = strtok (NULL, " ");
    }
    EOL = log_.size();
    cout << endl;
}
string GossipNode::getNewHashedLogs()
{
    string res;
    for (int  i = log_.size()-1; i> EOL -1; --i)
    {
        res+= " " + to_string(strHash(log_.at(i)));
    }
    return res;
}

} // cpisync
} // namespace niok
