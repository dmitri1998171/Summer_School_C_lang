#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include "protocol.h"    

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int CreateTCPSocket(unsigned short port)
{
    int sock;                        /* socket to create */
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&TCPechoServAddr, 0, sizeof(TCPechoServAddr));   /* Zero out structure */
    TCPechoServAddr.sin_family = AF_INET;                /* Internet address family */
    TCPechoServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* Any incoming interface */
    TCPechoServAddr.sin_port = htons(port);              /* Local port */

    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    return sock;
}

int CreateTCP2Socket(unsigned short port)
{
    int sock;                        /* socket to create */
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&TCP2echoServAddr, 0, sizeof(TCP2echoServAddr));   /* Zero out structure */
    TCP2echoServAddr.sin_family = AF_INET;                /* Internet address family */
    TCP2echoServAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* Any incoming interface */
    TCP2echoServAddr.sin_port = htons(port);              /* Local port */

    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    return sock;
}

int CreateTCPClientSocket(unsigned short port)
{
    int sock;                        /* socket to create */
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&TCPechoCliAddr, 0, sizeof(TCPechoCliAddr));   /* Zero out structure */
    TCPechoCliAddr.sin_family = AF_INET;                /* Internet address family */
    TCPechoCliAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* Any incoming interface */
    TCPechoCliAddr.sin_port = htons(port);              /* Local port */

    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    return sock;
}

int CreateTCPClient2Socket(unsigned short port)
{
    int sock;                        /* socket to create */
    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&TCPechoCli2Addr, 0, sizeof(TCPechoCli2Addr));   /* Zero out structure */
    TCPechoCli2Addr.sin_family = AF_INET;                /* Internet address family */
    TCPechoCli2Addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* Any incoming interface */
    TCPechoCli2Addr.sin_port = htons(port);              /* Local port */

    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    return sock;
}

int CreateUDPSocket(unsigned short port){
	int sock;

	/* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("UDPsocket() failed");

    /* Construct local address structure */
    memset(&UDPechoServAddr, 0, sizeof(UDPechoServAddr));   
    UDPechoServAddr.sin_family = AF_INET;                
    UDPechoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    UDPechoServAddr.sin_port = htons(port);      

    int bp = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &bp, sizeof(bp));
    
    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    
 	return sock; 
}

void UDPFunc(unsigned short port){
    int udpCliSock;
    
    udpCliSock = CreateUDPSocket(port);
    
    // Bind to the local address 
    if (bind(udpCliSock, (struct sockaddr *) &UDPechoServAddr, sizeof(UDPechoServAddr)) < 0){
        DieWithError("UDPbind() failed");}
    
    recv(udpCliSock, &udpbuf, sizeof(udpbuf), 0);
    printf("\tUDP: qmsgSize=%d\n", udpbuf);

    close(udpCliSock);
}

int AcceptTCPConnection(int servSock)
{
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */

    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);
    
    /* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
           &clntLen)) < 0)
        DieWithError("accept() failed");
    
    return clntSock;
}
