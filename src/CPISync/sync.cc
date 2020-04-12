/*
* Interactive CPISync process to be used in GossipNode.
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#include "../common/base/Base.h"
#include <CPISync/Syncs/GenSync.h>
#include <CPISync/Data/DataObject.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

/** useage:
argv[1] = HOST
argv[2] = "s" for server, "c" for client
argv[3] = NUM_CHAR
other argv = data
*/

int main(int argc, char *argv[])
{
    //constants
    const int PORT = 8001; // port on which to connect
    const int ERR = 0; // bypass error rate
    const int M_BAR = 1; // max differences between server and client
    const int PARTS = 3; // partitions per level for partition-syncs
    const int EXP_ELTS = 4; // expected number of elements per set
    const int METHOD = 0; // index of method to sync
    //user parameters
    const int NUM_CHAR = stoi(argv[3]); //max number chars in each object (20 for 64 bit hash)
    const string HOST = argv[1]; //host ip (use "172.28.1.1 for docker testing)
    GenSync genSync = GenSync::Builder().
                      setProtocol(GenSync::SyncProtocol::InteractiveCPISync).
                      setComm(GenSync::SyncComm::socket).
                      setPort(PORT).
                      setHost(HOST).
                      setErr(ERR).
                      setMbar(M_BAR).
                      setBits(NUM_CHAR*CHAR_BIT).
                      setNumPartitions(PARTS).
                      setExpNumElems(EXP_ELTS).
                      build();
    //add data to sync
    for (int i = 4; i<argc; ++i)
    {
        genSync.addElem(make_shared<DataObject>(argv[i]));
    }
    //sync
    if (strcmp(argv[2],"s")==0)
    {
        genSync.serverSyncBegin(0);
    }
    if (strcmp(argv[2],"c")==0)
    {
        genSync.clientSyncBegin(0);
    }
    //stdout output is set difference
    list<string> elems = genSync.dumpElements();
    int diff = elems.size()-argc+4;
    for (int i = 0; i<diff; ++i)
    {
        cout <<" "<< elems.back();
        elems.pop_back();
    }
}

