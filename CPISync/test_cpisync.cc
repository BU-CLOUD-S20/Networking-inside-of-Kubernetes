#include <iostream>
#include <string>
#include <CPISync/Syncs/GenSync.h>
#include <CPISync/Data/DataObject.h>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {

    GenSync::SyncProtocol prot;
    
    prot = GenSync::SyncProtocol::InteractiveCPISync;

    const int PORT = 8001; // port on which to connect
    const int ERR = 8; // inverse log of error chance
    const int M_BAR = 1; // max differences between server and client
    const int BITS = CHAR_BIT; // bits per entry
    const int PARTS = 3; // partitions per level for partition-syncs
    const int EXP_ELTS = 4; // expected number of elements per set

    const int METHOD = 0; // index of method to sync
    GenSync genSync = GenSync::Builder().
			setProtocol(prot).
			setComm(GenSync::SyncComm::socket).
			setPort(PORT).
			setErr(ERR).
			setMbar(M_BAR).
			setBits((prot == GenSync::SyncProtocol::IBLTSync || prot == GenSync::SyncProtocol::OneWayIBLTSync ? BITS : BITS * CHAR_BIT)).
			setNumPartitions(PARTS).
			setExpNumElems(EXP_ELTS).
            build();

    genSync.addElem(make_shared<DataObject>('a'));
    genSync.addElem(make_shared<DataObject>('b'));
    genSync.addElem(make_shared<DataObject>('c'));

    genSync.addElem(make_shared<DataObject>('d'));

    cout << "listening on port " << PORT << "..." << endl;
    genSync.clientSyncBegin(0);
    cout << "sync succeeded." << endl;
}
