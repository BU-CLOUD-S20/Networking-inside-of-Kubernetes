/*
* NIOK benchmark
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/

#include "GossipNode.h"
#include <memory.h>
#include <chrono>
#include "../common/base/Base.h"

using namespace std::chrono;
using namespace niok;
using namespace cpisync;
using std::cout;
using std::endl;
using std::string;
using std::thread;

GossipNode* currentNode;

//sync parameters
string IP_1, IP_2;

//processing context
const int NUM_CHAR_HASH = 20; //default 20
const int NUM_CHAR_ENTRY = 256;
hash<string> strHash;
/**
Test used for making a binary tree of gossip nodes. All times in ms.
serverOnly('s') - root node, IP_1 and IP_2 are children, input operations into this node
serverClient('sc') - intermediate node, IP_1 and IP_2 are children
clientOnly - leaf node
*/
int main(int argc, char *argv[])
{
    //==============================================================================================
    //handle incorrect usage
    if (argc > 4)
    {
        cout << "NAME:" << endl << "   bench - Benchmarking tool for niok" << endl;
        cout << "USAGE:" << endl << "   bench [s/c/sc] [IP_1] [IP_2]"<< endl;
        exit (EXIT_FAILURE);
    }
    //==============================================================================================

    //create a node
    currentNode = new GossipNode(strHash, NUM_CHAR_HASH, NUM_CHAR_ENTRY, {});
    //determine server/client/server and client state
    if (strcmp(argv[1],"s")==0)
    {
        IP_1 = argv[2];
        IP_2 = argv[3];
        while(true)
        {
            bool doLogs = false;
            cout << "server: ";
            char input[256];
            cin.getline(input, 256, '\n');
            //parse input
            vector<string> inputVec;
            char* tok = strtok(input, " ");
            while (tok!= NULL)
            {
                inputVec.push_back(tok);
                tok = strtok (NULL, " ");
            }
            //execute operation
            if (inputVec[0].compare("put")==0)
            {
                currentNode->put(inputVec[1], inputVec[2]);
                doLogs = true;
            }
            else if (inputVec[0].compare("del")==0)
            {
                currentNode->remove(inputVec[1]);
                doLogs = true;
            }
            else if (inputVec[0].compare("clear")==0)
            {
                system("clear");
            }

            else
            {
                cout << "Please use one of the following commands:" << endl;
                cout << "1. put [key] [value]" <<endl;
                cout << "2. del [key]" <<endl;
                cout << "3. clear" <<endl;
                cout << endl;
            }
            if (doLogs)
            {
                auto start = high_resolution_clock::now();
                auto startTime = duration_cast<milliseconds>(start.time_since_epoch());
                cout << "serverOnly node sync start time: " << startTime.count() << endl;
                //process
                currentNode->processLogEntry();
                //sync
                currentNode->sync(unordered_set<string>({IP_1,IP_2}), false);
                //process after sync
                currentNode->processLogEntry();
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop - start);
                cout << "serverOnly node sync duration: " << duration.count() << endl;
            }
        }
    }
    else if (strcmp(argv[1],"c")==0)
    {
        while(true)
        {
            //sync with parent
            currentNode->processLogEntry();
            currentNode->sync(currentNode->ip_, true);
            currentNode->processLogEntry();
            auto stopTime = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
            cout << "clientOnly node sync end time: " << stopTime.count()<< endl;
        }
    }
    else if (strcmp(argv[1],"sc")==0)
    {
        IP_1 = argv[2];
        IP_2 = argv[3];
        while(true)
        {
            //sync with parent
            currentNode->processLogEntry();
            currentNode->sync(currentNode->ip_, true);
            currentNode->processLogEntry();
            //sync with child
            currentNode->sync(unordered_set<string>({IP_1, IP_2}), false);
            //process after sync
            currentNode->processLogEntry();
            cout<< "serverClient node synced" <<endl;
        }
    }
}
