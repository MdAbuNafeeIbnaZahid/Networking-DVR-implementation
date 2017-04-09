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

#define SIZE 1024


using namespace std;

long long twoByteNum(char *str)
{
    long long a, b, c, e, d, ret = 0;
    ret =  ( ( unsigned char ) str[1] << 8 ) | ( (unsigned char) str[0] );
    return ret;
}

long long ifMatchUpTo(const char *str1, const char *str2, long long upTo)
{
    long long a, b, c, d, ret = 1;
    for (a = 0; a < upTo; a++)
    {
        if ( str1[a] != str2[a] )
        {
            ret = 0;
        }
    }
    return ret;
}

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

int sockFd, bindFlag, numBytesReceived;
struct sockaddr_in myAddr, sendToAddr, recvFromAddr;
set<string> allNodes;
map<string, int> neighborDist;
map<string, int> neighborLastClock;

string getIPV4(char *str)
{
    long long a, b, c, d, e;
    string ret;
    for ( a = 0; a < 4; a++ )
    {
        ret = ret + LLToStr( (unsigned char) ( *( str+a ) ) );
        if ( a < 3 )
        {
            ret = ret + ".";
        }
    }
    return ret;
}

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
char buffer[SIZE];

long long sendStrTo(string ipAddress, string str)
{
    long long a, b, c, d, e, f, ret;
    struct sockaddr_in sendToAddr;
    sendToAddr.sin_family = AF_INET;
    sendToAddr.sin_port = htons(4747);
    inet_pton(AF_INET, ipAddress.c_str(), &sendToAddr.sin_addr.s_addr);
    ret = sendto(sockFd, str.c_str(), 1024, 0, (struct sockaddr*) &sendToAddr, sizeof(sockaddr_in));
    return ret;
}

long long sendRoutingTableToAllNeighbor()
{
    long long a, b, c, d, e;
    struct sockaddr_in neighborAddr;
    map<string, int>::iterator mapStringToIntIt;
    for (mapStringToIntIt = neighborDist.begin(); mapStringToIntIt != neighborDist.end(); mapStringToIntIt++)
    {
        neighborAddr.sin_family = AF_INET;
        neighborAddr.sin_port = htons(4747);
    }
}

int main(int argc, char *argv[])
{
    long long a, b, c, d,e, f;

    string node1, node2, headStr;

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

    socklen_t addrLen = sizeof( recvFromAddr );
    while(1)
    {
        numBytesReceived = recvfrom(sockFd, buffer, 1024, 0, (struct sockaddr*) &recvFromAddr, &addrLen);
        cout << buffer << endl;
        if ( ifMatchUpTo(buffer, "clk", 3) )
        {
            cout << "clock came" << endl;
            clockCnt++;
        }
        else if ( ifMatchUpTo(buffer, "cost", 4) )
        {
            cout << "cost came " << endl;
            string ip1 = getIPV4( buffer + 4 );
            string ip2 = getIPV4( buffer + 8 );
            long long newCost = twoByteNum( buffer + 12 );
            if ( ip2 == myNode )
            {
                swap(ip1, ip2);
            }
            cout << ip1 << " " << ip2 << " " << newCost <<  endl;
        }
        else if ( ifMatchUpTo(buffer, "send", 4) )
        {
            cout << "send came" << endl;
        }
        else if ( ifMatchUpTo(buffer, "show", 4) )
        {
            cout << "show came " << endl;
            cout << myRoutingTable.toString() << endl;
        }
        else if ( ifMatchUpTo(buffer, "frwd", 4) )
        {
            cout << "frwd came " << endl;
        }
        else
        {
            cout << "routing table came " << endl;
        }
    }

    return 0;
}
