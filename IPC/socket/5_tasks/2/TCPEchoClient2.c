#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 32   /* Size of receive buffer */

struct sockaddr_in echoServAddr; /* Echo server address */
struct sockaddr_in echoClntAddr; /* Client address */
unsigned short echoServPort;     /* Echo server port */
char *servIP;                    /* Server IP address (dotted quad) */

void DieWithError(char *errorMessage, int I);  /* Error handling function */

void setSocet(int *sock, int port, struct sockaddr_in addr, int I){
    /* Create a reliable, stream socket using TCP */
    if ((*sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed", I);

    /* Construct the server address structure */
    memset(&addr, 0, sizeof(addr));     /* Zero out structure */
    addr.sin_family      = AF_INET;             /* Internet address family */
    addr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    addr.sin_port        = htons(port);         /* Server port */

    /* Establish the connection to the echo server */
    if (connect(*sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        DieWithError("connect", I);
        
}
int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */

    if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoString = argv[2];         /* Second arg: string to echo */

    if (argc == 4)
        echoServPort = atoi(argv[3]); /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */

    setSocet(&sock, echoServPort, echoServAddr, 1);

	int port = 0;
	recv(sock, &port, sizeof(int),0);
	printf("port: %d\n\n", port);

    close(sock);

    int new_sock;
    setSocet(&new_sock, port, echoClntAddr, 2);

    int num = 10;
    if (send(new_sock, &num, sizeof(int), 0) != sizeof(int))
        DieWithError("send", 2);
    printf("send_num: %d\n\n", num);

    exit(0);
}
