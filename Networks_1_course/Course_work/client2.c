#include "TCPEchoServer.h"
#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>
#include "protocol.h"   

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    srand(time(NULL));

    while(1){ 

        UDPFunc(UDPPORT2);
        
        if(udpbuf > 0){
            int tcpClisock;   
            struct protocol proto;

            tcpClisock = CreateTCPClient2Socket(TCPPORT2);

            /* Establish the connection to the echo server */
            if (connect(tcpClisock, (struct sockaddr *) &TCPechoCli2Addr, sizeof(TCPechoCli2Addr)) < 0)
                DieWithError("TCPconnect() failed");

            if (recv(tcpClisock, &proto, proto.len, 0) < 0)
                DieWithError("TCPrecv() failed");
            printf("TCP: time=%d len=%d text=%s\n", proto.time, proto.len, proto.text);

            close(tcpClisock);
            sleep(proto.time);
        }
    }
  
    exit(0);
}

