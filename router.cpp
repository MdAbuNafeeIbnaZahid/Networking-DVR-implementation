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

string LLToStr(long long num )
{
    string ret;
    long long dig;
    while( num )
    {
        dig = num%10;
        ret.push_back( '0' + dig );
        num /= 10;
    }
    reverse( ret.begin(), ret.end() );
    return ret;
}

struct routingTableRow
{
    //string owner;
    string destination;
    string nextHop;
    int dis;
    routingTableRow(){}
    routingTableRow(string destination, string nextHop, int dis)
    {
        this->destination = destination;
        this->nextHop = nextHop;
        this->dis = dis;
    }
    string toStr(  )
    {
        string disStr = LLToStr( dis );
        string ret = destination;
        ret = ret + " " + nextHop + " " + disStr;;
        return ret;
    }
};

struct routingTable
{
    string owner;
    map< string, routingTableRow > table;
    routingTable(){}

    void initRoutingTable(string owner, set<string> allNodes, map<string, int> neighborDist)
    {
        this->owner = owner;
        set<string>::iterator setStrIt;
        map<string, int>::iterator mapStrToIntIt;
        for ( setStrIt = allNodes.begin(); setStrIt != allNodes.end(); setStrIt++ )
        {
            if ( *setStrIt == owner )
            {
                continue;
            }
            table[ *setStrIt ] = routingTableRow(*setStrIt, "NONE", INT_MAX);
        }
        for ( mapStrToIntIt = neighborDist.begin(); mapStrToIntIt != neighborDist.end(); mapStrToIntIt++ )
        {
            table[ mapStrToIntIt->first ].nextHop = mapStrToIntIt->first;
            table[ mapStrToIntIt->first ].dis = mapStrToIntIt->second;
        }
    }

    string toString()
    {
        string ret = owner + "\n" ;
        map<string, routingTableRow>::iterator it;
        for ( it = table.begin(); it != table.end(); it++ )
        {
            ret = ret + (it->second).toStr() + "\n";
        }
        return ret;
    }
};

set<string> allNodes;
map<string, int> neighborDist;
map<string, int> neighborLastClock;

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

int clockCnt;
routingTable myRoutingTable;
string myNode;

int main(int argc, char *argv[])
{
    long long a, b, c, d,e, f;
    int sockFd, bindFlag, numBytesReceived;
    struct sockaddr_in myAddr, sendToAddr, recvFromAddr;
    string node1, node2;

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
            neighborLastClock[ node2 ] = clockCnt;
        }
    }



    printSetStr( allNodes );
    printMapStrToInt( neighborDist );

    myRoutingTable.initRoutingTable( myNode, allNodes, neighborDist );

    cout << myRoutingTable.toString() << endl;

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
