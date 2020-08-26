#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/msg.h>

#define TCPPORT1 1122
#define TCPPORT2 1133
#define UDPPORT1 2211
#define UDPPORT2 2233

#define MAX_LEN_MSG 15
#define K 5
#define L 3

#define MAX_SLEEP_TIME 10
#define MAX_LEN 32
#define MAXPENDING 5

int udpbuf;

int qmsgSize;

//int clntSock; 
int tcpServSock;                        
int udpServsock;                        
struct sockaddr_in UDPechoServAddr;

struct sockaddr_in TCPechoServAddr;
struct sockaddr_in TCP2echoServAddr;

struct sockaddr_in TCPechoCliAddr;
struct sockaddr_in TCPechoCli2Addr;

struct msqid_ds q;

struct protocol {
  int time;
  int len;
  char text[MAX_LEN+1];
};

