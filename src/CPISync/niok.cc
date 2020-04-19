/*
* Command Line Interface for NIOK
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/


#include "GossipNode.h"
#include <memory.h>
#include <thread>

using namespace niok;
using namespace cpisync;
using std::cout;
using std::endl;
using std::string;
using std::thread;

GossipNode* currentNode;

const string HOST = "172.28.1.1";
const int NUM_CHAR_HASH = 20; //default 20
const int NUM_CHAR_ENTRY = 256;
hash<string> strHash;

int main(int argc, char *argv[])
{
    const int NODE = stoi(argv[1]);
    //==============================================================================================
    //handle incorrect usage
    if (argc < 2)
    {
        cout << "NAME:" << endl << "   niok - Command Line Interface for niok." << endl;
        cout << "USAGE:" << endl << "   niok [node name] [initial elements]"<< endl;
        exit (EXIT_FAILURE);
    }
    //==============================================================================================
    //create a node
    const string name = argv[1];
    vector<string> initialElems;
    for (int i = 2; i< argc; ++i)
    {
        initialElems.push_back(argv[i]);
    }
    currentNode = new GossipNode(name, 0, "/tmp/"+name,
                                 strHash, NUM_CHAR_HASH, NUM_CHAR_ENTRY, initialElems);

    //use ./niok 1 or ./niok 2
    bool server;
    if (NODE == 1)
        server = true;
    else
        server = false;
    while (true)
    {
        //get input
        cout << name << ": ";
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
        }
        else if (inputVec[0].compare("get")==0)
        {
            string res;
            currentNode->get(inputVec[1], &res);
            cout << res << endl;
        }
        else if (inputVec[0].compare("del")==0)
        {
            currentNode->remove(inputVec[1]);
        }
        else if (inputVec[0].compare("sync")==0)
        {
        }
        else
        {
            cout << "Please use one of the following commands:" << endl;
            cout << "1. put [key] [value]" <<endl;
            cout << "2. get [key]" <<endl;
            cout << "3. del [key]" <<endl;
        }
        //process
        currentNode->processLogEntry();
        //sync
        currentNode->sync(HOST, server);
        //process after sync
        currentNode->processLogEntry();
    }
    //delete
    delete currentNode;
}
