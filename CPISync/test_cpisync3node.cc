/*
* An Interactive CPISync example with 3 nodes. Implemented using leveldb.
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#include "GossipNode.h"

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
                      setHost("172.28.1.1").
                      setErr(ERR).
                      setMbar(M_BAR).
                      setBits(BITS * sizeof(char)).
                      setNumPartitions(PARTS).
                      setExpNumElems(EXP_ELTS).
                      build();
    //==============================================================================================
    //sync current server-client pair
    const string key = "001";
    string name = "node";
    name.append(argv[1]);
  //  Gossip::getNodeValues({"node1","node2","node3"}, key);
    if (NODE==1)
    {
        GossipNode server;
        server.init(name, key, "abcef", genSync);
        server.connect(genSync);
    }
    else
    {
        GossipNode client;
        switch(NODE)
        {
            case(2):
                client.init(name, key, "abcd", genSync);
            break;
            default:
            client.init(name, key, "abcg", genSync);
        }
        client.listen(genSync);
    }
}
