#include <bits/stdc++.h>
#include <cstdio>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>


#include <cstring>
#include <cstdlib>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


using namespace std;

struct routingTableEntry
{
    //string owner;
    string destination;
    string nextHop;
    int dis;
};

set<string> allNodes;
map<string, int> neighborDist;

void printSetStr( set<string>  setStr )
{
    cout << endl;
    set<string>::iterator setStrIt;
    for ( setStrIt = setStr.begin(); setStrIt != setStr.end(); setStrIt++ )
    {
        cout << *setStrIt << endl;
    }
    cout << endl;
}

void printMapStrToInt(map<string, int> mapStrToInt)
{
    long long a, b, c, d, e;
    map<string, int>::iterator it;
    cout << endl;
    for (it = mapStrToInt.begin(); it != mapStrToInt.end(); it++)
    {
        cout << it->first << " " << it->second << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    long long a, b, c, d,e, f;
    int sockFd, bindFlag, numBytesReceived;
    struct sockaddr_in myAddr, sendToAddr, recvFromAddr;
    string node1, node2;
    string myNode;
    cout << "argc = " << argc << endl;
    if ( argc < 3 )
    {
        cout << "error argc < 3" << endl << endl;
        return 0;
    }
    myNode = argv[1];
    ifstream topoRead;
    topoRead.open( argv[2] );
    while( topoRead >> node1 )
    {
        int dist;
        topoRead >> node2;
        topoRead >> dist;
        allNodes.insert( node1 );
        allNodes.insert( node2 );
        if ( node2 == myNode )
        {
            swap(node1, node2);
        }
        if ( node1 == myNode )
        {
            neighborDist[ node2 ] = dist;
        }
    }

    printSetStr( allNodes );
    printMapStrToInt( neighborDist );
    sockFd = socket(AF_INET, SOCK_DGRAM, 0);

    myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(4747);
	//server_address.sin_addr.s_addr = inet_addr("192.168.10.100");
	inet_pton(AF_INET, argv[1], &myAddr.sin_addr);

    bindFlag = bind(sockFd, (struct sockaddr*) &myAddr, sizeof(sockaddr_in) );
    if(bindFlag==0)
    {
        printf("successful bind\n\n");
    }
    else
    {
        cout << "unsuccessful bind" << endl << endl;
    }

    while(1)
    {
        //numBytesReceived =
    }

    return 0;
}
