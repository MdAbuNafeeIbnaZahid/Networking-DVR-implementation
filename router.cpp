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

vector<string> myStrTokenizer(string givenStr, vector<char> delimVec)
{
    long long a, b, c, d, e;
    vector<string> ret;
    string strToPush;
    for ( a = 0; a < givenStr.size(); a++ )
    {
        if ( find( delimVec.begin(), delimVec.end(), givenStr[a] ) != delimVec.end() ) // delim found
        {
            if ( strToPush.size() )
            {
                ret.push_back( strToPush );
            }
            strToPush = "";
        }
        else
        {
            strToPush.push_back( givenStr[a] );
        }
    }
    if ( strToPush.size() )
    {
        ret.push_back( strToPush );
    }
    return ret;
}

long long strToLL(string str)
{
    long long a, b, c, d, e, ret = 0;
    cout << "in strToLL" << endl;
    cout << "str = " << str << endl;
    for (a = 0;  a < str.size(); a++)
    {
        ret = ret * 10 + ( str[a] - '0' );
    }
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
    routingTableRow(string str)
    {
        cout << "in routingTableRow" << endl;
        cout << "str = " << str << endl;
        vector<char> delimVec;
        delimVec.push_back( ' ' );
        vector<string> vecStr = myStrTokenizer(str, delimVec );
        this->destination = vecStr[0];
        this->nextHop = vecStr[1];
        cout << "destination and nextHop assigned " << endl;
        cout << "vecStr[2] = " << vecStr[2] << endl;
        this->dis = strToLL( vecStr[2] );
        cout << "dis assigned " << endl;
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
    routingTable(string routingTableInStr)
    {
        long long a, b, c, d, e;
        vector<char> delimVec;
        delimVec.push_back( '\n' );
        vector<string> tokens = myStrTokenizer(routingTableInStr, delimVec);
        cout << "tokens in routing table construction " << endl;
        for ( long long a = 0; a < tokens.size(); a++ )
        {
            cout << tokens[a] << endl;
        }
        owner = tokens[0];
        for (a = 1; a < tokens.size(); a++)
        {
            cout << "a = " << a << endl;
            routingTableRow rtr = routingTableRow( tokens[a] );
            cout << "row constructed " << endl;
            table[ rtr.destination ] = rtr;
        }
    }
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

    string toStr()
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

void loadStrInCharAr(string str, char *charAr)
{
    long long a, b, c, d, e;
    for ( a = 0; a < str.size(); a++ )
    {
        *(charAr+a) = str[a];
    }
    *(charAr+a) = '\0';
}

long long sendStrTo(string ipAddress, string str)
{
    long long a, b, c, d, e, f, ret;
    struct sockaddr_in sendToAddr;
    sendToAddr.sin_family = AF_INET;
    sendToAddr.sin_port = htons(4747);
    inet_pton(AF_INET, ipAddress.c_str(), &sendToAddr.sin_addr.s_addr);
    char buffer[SIZE];
    memset(buffer, 0, sizeof(buffer) );
    loadStrInCharAr(str, buffer);
//    cout << "in sendStrTo" << endl;
//    cout << "ipAddress = " << ipAddress << endl;
//    cout << "buffer = " << endl;
//    cout << buffer ;
    ret = sendto(sockFd, buffer, SIZE, 0, (struct sockaddr*) &sendToAddr, sizeof(sockaddr_in));
    return ret;
}

long long sendRoutingTableToAllNeighbor()
{
    long long a, b, c, d, e;
    struct sockaddr_in neighborAddr;
    map<string, int>::iterator mapStringToIntIt;
    for (mapStringToIntIt = neighborDist.begin(); mapStringToIntIt != neighborDist.end(); mapStringToIntIt++)
    {
//        neighborAddr.sin_family = AF_INET;
//        neighborAddr.sin_port = htons(4747);
//        inet_pton(AF_INET, mapStringToIntIt->first, &server_address.sin_addr.s_addr);
//        cout << "mapStringToIntIt->first = " << mapStringToIntIt->first << endl;
//        cout << "myRoutingTable.toStr() = " << myRoutingTable.toStr() << endl;
        sendStrTo( mapStringToIntIt->first, myRoutingTable.toStr() );
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

    string myRoutingTableString = myRoutingTable.toStr();
    cout << myRoutingTableString << endl;
    routingTable copyRT = routingTable( myRoutingTableString );
    cout << "copy of routing table assigned " << endl;
    cout << copyRT.toStr() << endl;
    cout << "copy printed" << endl;

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
        memset(buffer, 0, sizeof(buffer) );
        numBytesReceived = recvfrom(sockFd, buffer, 1024, 0, (struct sockaddr*) &recvFromAddr, &addrLen);
        cout << buffer << endl;
        if ( ifMatchUpTo(buffer, "clk", 3) )
        {
            //cout << "clock came" << endl;
            clockCnt++;
            sendRoutingTableToAllNeighbor();
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
            //cout << ip1 << " " << ip2 << " " << newCost <<  endl;
        }
        else if ( ifMatchUpTo(buffer, "send", 4) )
        {
            cout << "send came" << endl;
        }
        else if ( ifMatchUpTo(buffer, "show", 4) )
        {
            cout << "show came " << endl;
            cout << myRoutingTable.toStr() << endl;
        }
        else if ( ifMatchUpTo(buffer, "frwd", 4) )
        {
            cout << "frwd came " << endl;
        }
        else
        {
            cout << "routing table came " << endl;
            routingTable receivedRoutingTable = routingTable(buffer);
            cout << receivedRoutingTable.toStr() << endl;
        }
    }

    return 0;
}
