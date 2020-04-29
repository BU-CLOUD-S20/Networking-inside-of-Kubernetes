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

bool serverClient = false;
bool clientOnly = false;
bool serverOnly = false;
const int NUM_CHAR_HASH = 20; //default 20
const int NUM_CHAR_ENTRY = 256;
hash<string> strHash;


int main(int argc, char *argv[])
{
    //==============================================================================================
    //handle incorrect usage
    if (argc < 2)
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
        serverOnly = true;
    }
    else if (strcmp(argv[1],"c")==0)
    {
        clientOnly = true;

    }
    else if (strcmp(argv[1],"sc")==0)
    {
        serverClient = true;
    }
    //parse input

    if(serverOnly)
    {
        //get input
        cout << currentNode->name_ << ": ";
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
        else
        {
            cout << "Please use one of the following commands:" << endl;
            cout << "1. put [key] [value]" <<endl;
            cout << "2. get [key]" <<endl;
            cout << "3. del [key]" <<endl;
            cout << "4. show (show neighbors of current node)" << endl;

        }
    }
    //process
    currentNode->processLogEntry();
    //sync
    if (serverOnly)
    {
        auto start = high_resolution_clock::now();
        currentNode->sync(argv[2], true);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout<< "synced as " << argv[2] << endl;;
        cout << "Time for operation was "<< duration.count() << " microseconds" << endl;
    }
    else if (serverClient)
    {
        currentNode->sync(argv[2], false);
        cout << "synced with " << argv[2] <<endl;
        currentNode->sync(argv[3], true);
        cout<< "synced as " << argv[3] << endl;
    }
    else
    {
        currentNode->sync(argv[2], false);
        cout << "synced with " << argv[2] << endl;
    }
    //process after sync
    currentNode->processLogEntry();

}
