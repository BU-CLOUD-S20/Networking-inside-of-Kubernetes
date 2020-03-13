#include "GossipUtilities.h"

namespace niok
{
vector<string> Gossip::getElems(GenSync &genSync)
{
    vector<string> res;
    for (string dop : genSync.dumpElements()) //optimization: go from the end and only update log until you hit a preexiting entry
    {
        res.push_back(dop);
        cout<< dop << " ";
    }
    cout << endl;
    return res;
}
}
