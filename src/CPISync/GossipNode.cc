#include "GossipNode.h"
#include "../kvstore/LevelEngine.h"
#include "../common/base/Base.h"

namespace niok {
namespace cpisync {

void GossipNode::init(string nodeName, vector<string> initialElems, GenSync& genSync)
{
    //db = Gossip::getDB(name);
    //if key's value if empty initialize
    if (true) //TODO do this step only if creating a new node
    {
        cout << endl<< "init "<<nodeName<< " data: " <<endl <<endl;
        Gossip::putElems(initialElems, genSync);
        Gossip::getElems(genSync);
    }
}
void GossipNode::connect(GenSync& genSync)
{
    cout << endl << "connecting..." << endl << endl;
    genSync.serverSyncBegin(0);
    cout << endl << "synced " << name_ << " data: " <<endl << endl;
    Gossip::getElems(genSync);
}
void GossipNode::listen(GenSync& genSync)
{
    cout << endl << "listening..." << endl << endl;
    genSync.clientSyncBegin(0);
    cout << "synced " << name_ << " data: " <<endl;
    Gossip::getElems(genSync);
}

bool GossipNode::get(const std::string& key, std::string* value) {
    auto resultCode = db_->get(key, value);
    if (resultCode != kvstore::ResultCode::SUCCEEDED) {
        cout << resultCode << endl;
        return false;
    } else {
        return true;
    }
}

bool GossipNode::put(std::string key, std::string value) {
    auto resultCode = db_->put(key, value);
    if (resultCode != kvstore::ResultCode::SUCCEEDED) {
        cout << resultCode << endl;
        return false;
    } else {
        return true;
    }
}

} // cpisync
} // namespace niok