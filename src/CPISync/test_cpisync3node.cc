/*
* An Interactive CPISync example with 3 nodes. Implemented using leveldb.
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#include "GossipNode.h"
#include <memory.h>

using namespace niok;
using namespace cpisync;
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
    //sync current server-client pair
    const string HOST = "172.28.1.1";
    const int NUM_CHAR = 64;
    hash<string> strHash;
    string name = "node";
    name.append(argv[1]);
    GossipNode* currentNode;
    if (NODE==1)
    {
        currentNode = new GossipNode(name, 0, "/tmp/GossipNodeTest.XXXXXX", strHash, {});
        currentNode->GossipNode::put("3", "22");
        currentNode->GossipNode::put("2", "55");
        currentNode->GossipNode::remove("2");
    }
    else
    {
        switch(NODE)
        {
            case(2):
                currentNode = new GossipNode(name, 0, "/tmp/GossipNodeTest.XXXXXX", strHash, {});
            break;
            default:
                 currentNode = new GossipNode(name, 0, "/tmp/GossipNodeTest.XXXXXX", strHash,{"A", "B", "C"});
            break;
        }
        currentNode->sync(HOST, NUM_CHAR, false);
        currentNode->sync(HOST, NUM_CHAR, false);
        currentNode->sync(HOST, NUM_CHAR, false);
        currentNode->processLogEntry();

        string res;
        currentNode->GossipNode::get("3", &res);
        cout << res << endl;
        currentNode->GossipNode::get("2", &res);
        cout << res << endl;

    }
    delete currentNode;
    
}



