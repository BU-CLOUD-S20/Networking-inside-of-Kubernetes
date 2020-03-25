#include "GossipNode.h"

namespace niok
{
void GossipNode::init(string nodeName, vector<string> initialElems, GenSync& genSync)
{
    name = nodeName;
    db = getDB(name);
    //if key's value if empty initialize
    if (true) //TODO do this step only if creating a new node
    {
        DB* hashDefs = getDB("hashDefs");
        for (int i = 0; i < initialElems.size(); ++i)
        {
            log.push_back(initialElems[i]);
            string shash = std::to_string(strHash(initialElems[i]));
            hashDefs->Put(WriteOptions(), shash.substr(0, 17), initialElems[i]);
            //can only fit 17 chars in one object
            genSync.addElem(make_shared<DataObject>(shash.substr(0, 17)));
        }
        delete hashDefs;
        getElems(genSync);
        getLog();
    }
}
void GossipNode::connect(GenSync& genSync)
{
    cout << "connecting......................................." << endl << endl;
    genSync.serverSyncBegin(0);
    getDiff(genSync);
    getElems(genSync);
    getLog();
}
void GossipNode::listen(GenSync& genSync)
{
    cout << "listening........................................" << endl << endl;
    genSync.clientSyncBegin(0);
    getDiff(genSync);
    getElems(genSync);
    getLog();
}
void GossipNode::getDiff(GenSync& genSync)
{
    cout << "New hashed logs from last sync:" << endl <<endl;
    list<string> elems = genSync.dumpElements();
    EOL=log.size();
    DB* hashDefs = getDB("hashDefs");
    string s;
    while (elems.size()!=EOL)
    {
        cout << elems.back() << endl;
        hashDefs->Get(ReadOptions(), elems.back(), &s);
        log.push_back(s);
        elems.pop_back();
    }
    delete hashDefs;
    cout << endl;
}
void GossipNode::getElems(GenSync& genSync)
{
    cout << "All hashed logs of current node:" << endl << endl;
    for (string dop : genSync.dumpElements())
    {
        cout << dop << endl;
    }
    cout << endl;
}
void GossipNode::getLog()
{
    cout << "All logs of current node:" << endl << endl;
    for (int i = 0; i<log.size(); ++i)
    {
        cout <<log.at(i) << endl;
    }
    cout <<endl;
}
}
