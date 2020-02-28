/*
* An Interactive CPISync example with 3 nodes. Implemented using leveldb.
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#include "GossipUtilities.h"

using namespace leveldb;
using namespace niok;
using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
    //==============================================================================================
    //handle incorrect usage
    const int NODE = stoi(argv[1]);
    if (argc != 2 || NODE<1 || NODE>3)
    {
        cout << "Error: Use '3node 1' for server. Use '3node 2' or '3node 3' for clients" << endl;
        cout << "Run a server and a client in separate terminals or containers to sync." << endl;
        exit (EXIT_FAILURE);
    }
    //==============================================================================================
    //setup genSync
    const int PORT = 8001; // port on which to connect
    const int ERR = 8; // inverse log of error chance
    const int M_BAR = 1; // max differences between server and client
    const int BITS = CHAR_BIT; // bits per entry
    const int PARTS = 3; // partitions per level for partition-syncs
    const int EXP_ELTS = 4; // expected number of elements per set
    const int METHOD = 0; // index of method to sync
    GenSync genSync = GenSync::Builder().
                      setProtocol(GenSync::SyncProtocol::InteractiveCPISync).
                      setComm(GenSync::SyncComm::socket).
                      setPort(PORT).
                      setErr(ERR).
                      setMbar(M_BAR).
                      setBits(BITS * sizeof(char)).
                      setNumPartitions(PARTS).
                      setExpNumElems(EXP_ELTS).
                      build();
    //==============================================================================================
    //sync current server-client pair
    const string key = "001";
    Gossip::getNodeValues({"node1","node2","node3"}, key);
    if (strcmp(argv[1], "1")==0)
    {
        DB* db = Gossip::getDB("node1");
        string s;
        db->Get(ReadOptions(), key, &s);
        if (s.length()==0)
        {
            cout << "init node1 data: " <<endl;
            Gossip::putElems(genSync, "abcef");
            db->Put(WriteOptions(), key, Gossip::getElems(genSync));
        } else {
            Gossip::putElems(genSync, s);
            cout << "current node1 data: " <<endl;
            Gossip::getElems(genSync);
        }
        cout << "connecting on port " << PORT << "..." << endl;
        genSync.serverSyncBegin(0);
        cout << "synced node1 data: " <<endl;
        db->Put(WriteOptions(), key, Gossip::getElems(genSync));
        delete db;
    }
    else if(strcmp(argv[1], "2")==0)
    {
       DB* db = Gossip::getDB("node2");
        string s;
        db->Get(ReadOptions(), key, &s);
        if (s.length()==0)
        {
            cout << "init node2 data: " <<endl;
            Gossip::putElems(genSync, "abcd");
            db->Put(WriteOptions(), key, Gossip::getElems(genSync));
        } else {
            Gossip::putElems(genSync, s);
            cout << "current node2 data: " <<endl;
            Gossip::getElems(genSync);
        }
        cout << "listening on port " << PORT << "..." << endl;
        genSync.clientSyncBegin(0);
        cout << "synced node2 data: " <<endl;
        db->Put(WriteOptions(), key, Gossip::getElems(genSync));
        delete db;
    }
    else
    {
        DB* db = Gossip::getDB("node3");
        string s;
        db->Get(ReadOptions(), key, &s);
        if (s.length()==0)
        {
            Gossip::putElems(genSync, "abcg");
            db->Put(WriteOptions(), key, Gossip::getElems(genSync));
        } else {
            Gossip::putElems(genSync, s);
            Gossip::getElems(genSync);
        }
        cout << "listening on port " << PORT << "..." << endl;
        genSync.clientSyncBegin(0);
        db->Put(WriteOptions(), key, Gossip::getElems(genSync));
        cout << "sync succeeded." << endl;
    }

}
