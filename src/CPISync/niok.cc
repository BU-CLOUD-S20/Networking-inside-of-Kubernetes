/*
* Command Line Interface for NIOK
* Zhe Deng Networking Inside of Kubernetes EC528 Spring 2020
*/


#include "GossipNode.h"
#include <memory.h>
#include <thread>
#include "../common/base/Base.h"

using namespace niok;
using namespace cpisync;
using std::cout;
using std::endl;
using std::string;
using std::thread;

GossipNode* currentNode;

const int TCP_PORT = 8002;
const int NUM_CHAR_HASH = 20; //default 20
const int NUM_CHAR_ENTRY = 256;
hash<string> strHash;

void listenTCP(string ip, int port)
{
    while (true)
    {
        IPv4 *IP = new IPv4(ip, port);
        TCPServer *server = new TCPServer(IP);
        std::string res;
        server->start(res); 
        currentNode->addNeighbor(res);
    }
}

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
    vector<string> initialElems;
    for (int i = 2; i< argc; ++i)
    {
        initialElems.push_back(argv[i]);
    }
    currentNode = new GossipNode(strHash, NUM_CHAR_HASH, NUM_CHAR_ENTRY, initialElems);

    //use ./niok 1 or ./niok 2
    bool server;
    if (NODE == 1)
        server = true;
    else
        server = false;
    
    // TCP thread
    std::thread TCP_thread(listenTCP, currentNode->ip_, TCP_PORT);
    TCP_thread.detach();

    while (true)
    {
        // FLAG - need to process logs or not
        bool doLogs = false;
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
            doLogs = true;
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
            doLogs = true;
        }
        else if (inputVec[0].compare("sync")==0)
        {
        }
        else if (inputVec[0].compare("show")==0)
        {
            currentNode->showNeighbors();
        }
        else if (inputVec[0].compare("join")==0)
        {
            vector<string> ips;
            for (int i = 1; i < inputVec.size(); i++) 
            {
                ips.push_back(inputVec[i]);
            }
            currentNode->joinCluster(ips);
        }
        else
        {
            cout << "Please use one of the following commands:" << endl;
            cout << "1. put [key] [value]" <<endl;
            cout << "2. get [key]" <<endl;
            cout << "3. del [key]" <<endl;
            cout << "4. show (show neighbors of current node)" << endl;
        }

        if (doLogs) 
        {
            //process
            currentNode->processLogEntry();
            //sync
            currentNode->sync(currentNode->ip_, server);
            //process after sync
            currentNode->processLogEntry();
        }
        
    }
    //delete
    delete currentNode;
}
