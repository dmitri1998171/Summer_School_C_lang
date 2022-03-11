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

        UDPFunc(UDPPORT1);

        if(udpbuf >= 0){
            int tcpClisock;   
            struct protocol proto;

            tcpClisock = CreateTCPClientSocket(TCPPORT1);

            /* Establish the connection to the echo server */
            if (connect(tcpClisock, (struct sockaddr *) &TCPechoCliAddr, sizeof(TCPechoCliAddr)) < 0)
                DieWithError("TCPconnect() failed");

            proto.time = rand() % MAX_SLEEP_TIME;
            proto.len = rand() % (MAX_LEN);

            for (int i = 0; i < proto.len; i++)
                proto.text[i] = 65 + rand() % 60;
            proto.text[proto.len] = '\0';

            if (send(tcpClisock, &proto, sizeof(proto), 0) != sizeof(proto))
                DieWithError("send() sent a different number of bytes than expected");

            printf("TCP: time=%d len=%d text=%s\n", proto.time, proto.len, proto.text);
            sleep(proto.time);

            close(tcpClisock);
        } 
    }
  
    exit(0);
}
