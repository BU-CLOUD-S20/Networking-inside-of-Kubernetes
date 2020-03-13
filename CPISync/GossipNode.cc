#include "GossipNode.h"

namespace niok
{
void GossipNode::init(string nodeName, string nodeKey, string initialElems, GenSync& genSync)
{
    name = nodeName;
    key=nodeKey;
    db = Gossip::getDB(name);
    string s;
    db->Get(ReadOptions(), key, &s);
    //if key's value if empty initialize
    if (s.length()==0)
    {
        cout << endl<< "init "<<name<< " data: " <<endl <<endl;
        Gossip::putElems(genSync, initialElems);
        db->Put(WriteOptions(), key, Gossip::getElems(genSync));
    }
    else
    {
        Gossip::putElems(genSync, s);
        cout << endl<<"current "<< name << " data: " <<endl <<endl;
        Gossip::getElems(genSync);
    }
}
void GossipNode::connect(GenSync& genSync)
{
    cout << endl << "connecting..." << endl << endl;
    genSync.serverSyncBegin(0);
    cout << endl << "synced " <<name<< " data: " <<endl << endl;
    db->Put(WriteOptions(), key, Gossip::getElems(genSync));
}
void GossipNode::listen(GenSync& genSync)
{
    cout << endl << "listening..." << endl << endl;
    genSync.clientSyncBegin(0);
    cout << "synced " << name << " data: " <<endl;
    db->Put(WriteOptions(), key, Gossip::getElems(genSync));
}
}
