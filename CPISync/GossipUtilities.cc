#include "GossipUtilities.h"

namespace niok
{
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
