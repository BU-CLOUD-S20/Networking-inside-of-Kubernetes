#include "GossipUtilities.h"


namespace niok
{
void Node::init(string nodeName, string nodeKey, string initialElems, GenSync& genSync)
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
void Node::connect(GenSync& genSync)
{
    cout << endl << "connecting..." << endl << endl;
    genSync.serverSyncBegin(0);
    cout << endl << "synced " <<name<< " data: " <<endl << endl;
    db->Put(WriteOptions(), key, Gossip::getElems(genSync));
}
void Node::listen(GenSync& genSync)
{
    cout << endl << "listening..." << endl << endl;
    genSync.clientSyncBegin(0);
    cout << "synced " << name << " data: " <<endl;
    db->Put(WriteOptions(), key, Gossip::getElems(genSync));
}

string Gossip::getElems(GenSync &genSync)
{
    string str;
    for (string dop : genSync.dumpElements())
    {
        //print char data in proper format and save to 'str'
        char data = (char)(((int)dop[0])+2);
        cout<< data << " ";
        str.push_back(data);
    }
    cout << endl;
    return str;
}
void Gossip::getNodeValues(vector<string> nodes, string key)
{
    string s;
    cout << endl << "Current value of key '" << key << "' for all nodes:" << endl << endl;
    for (int i = 0; i<nodes.size(); ++i)
    {
        DB *db = nullptr;
        db = Gossip::getDB(nodes[i]);
        db->Get(ReadOptions(), key, &s);
        cout<< nodes[i] << ": " << s << endl;
        delete db;
    }
}
}
