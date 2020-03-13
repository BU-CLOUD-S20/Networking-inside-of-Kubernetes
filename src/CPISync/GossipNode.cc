#include "GossipNode.h"

namespace niok
{
void GossipNode::init(string nodeName, vector<string> initialElems, GenSync& genSync)
{
    name = nodeName;
    db = Gossip::getDB(name);
    //if key's value if empty initialize
    if (true) //TODO do this step only if creating a new node
    {
        cout << endl<< "init "<<name<< " data: " <<endl <<endl;
        Gossip::putElems(initialElems, genSync);
        Gossip::getElems(genSync);
    }
}
void GossipNode::connect(GenSync& genSync)
{
    cout << endl << "connecting..." << endl << endl;
    genSync.serverSyncBegin(0);
    cout << endl << "synced " <<name<< " data: " <<endl << endl;
    Gossip::getElems(genSync);
}
void GossipNode::listen(GenSync& genSync)
{
    cout << endl << "listening..." << endl << endl;
    genSync.clientSyncBegin(0);
    cout << "synced " << name << " data: " <<endl;
    Gossip::getElems(genSync);
}
}
