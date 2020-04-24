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

const int TCP_PORT = 8003;
const int NUM_CHAR_HASH = 20; //default 20
const int NUM_CHAR_ENTRY = 256;
hash<string> strHash;

void listenTCP(string ip, int port)
{
    IPv4 *IP = new IPv4(ip, port);
    TCPServer *server = new TCPServer(IP);
    std::string res;
    while (true)
    {
        server->start(res);
        currentNode->addNeighbor(res);
        server->stop();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void cpi()
{
    while (true)
    {
     currentNode->processLogEntry();
        currentNode->sync(currentNode->ip_, true);
        currentNode->processLogEntry();
    }
}

int main(int argc, char *argv[])
{
    //==============================================================================================
    //help
    if (argc == 2 && strcmp(argv[1],"help")==0)
    {
        cout << "NAME:" << endl << "   niok - Command Line Interface for niok." << endl;
        cout << "USAGE:" << endl << "   ./niok [initial elements]"<< endl;
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
    // TCP thread
    std::thread tcp_thread(listenTCP, currentNode->ip_, TCP_PORT);
    // CPI server thread
    std::thread cpi_thread(cpi);
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
            if (currentNode->get(inputVec[1], &res))
                cout << inputVec[1] << " : " << res << endl;
            cout << endl;
        }
        else if (inputVec[0].compare("del")==0)
        {
            currentNode->remove(inputVec[1]);
            doLogs = true;
        }
        else if (inputVec[0].compare("sync")==0)
        {
            doLogs = true;
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
        else if (inputVec[0].compare("clear")==0)
        {
            system("clear");
        }
        else if (inputVec[0].compare("exit")==0)
        {
            delete currentNode;
            cout << "---Thanks for using NIOK! See you" << endl;
            cout << endl;
            exit(0);
        }
        else
        {
            cout << "Please use one of the following commands:" << endl;
            cout << "1. put [key] [value]" <<endl;
            cout << "2. get [key]" <<endl;
            cout << "3. del [key]" <<endl;
            cout << "4. join [IPs]" << endl;
            cout << "5. show (show neighbors of current node)" << endl;
            cout << "6. exit" << endl;
            cout << endl;
        }

        if (doLogs)
        {
            //process
            currentNode->processLogEntry();
            currentNode->sync(currentNode->neighbors_, false);
            //process after sync
            currentNode->processLogEntry();
        }
    }
    tcp_thread.join();
    cpi_thread.join();

    //delete

}
