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
const int NUM_CHAR_ENTRY = 512;
hash<string> strHash;

void getInput()
{
    string input;
    while (true)
    {
        char input[256];
        cin.getline(input, 256, '\n');
        vector<string> inputVec;
        char* tok = strtok(input, " ");
        while (tok!= NULL)
        {
            inputVec.push_back(tok);
            tok = strtok (NULL, " ");
        }
        if (inputVec[0].compare("put")==0)
        {
            currentNode->put("2", "3");
        }
        else if (inputVec[0].compare("get")==0)
        {
            string res;
            currentNode->get(inputVec[1], &res);
            cout << res << endl;
        }
        currentNode->sync(HOST, true);
    }
}

int main(int argc, char *argv[])
{
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
                                 strHash, NUM_CHAR_HASH, NUM_CHAR_HASH, initialElems);
    //get input asynchronously
    thread inputThread(getInput);
    //listen on main thread
    while(true)
    {
        currentNode->sync(HOST, false);
        currentNode->processLogEntry();
        string res;
        currentNode->get("2", &res);
        cout << res << endl;
    }
    //delete
    delete currentNode;
}












