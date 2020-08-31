#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/wait.h>

struct sockaddr_in ServAddr;     /* Local address */
struct sockaddr_in ClntAddr;     /* Client address */
struct sockaddr_in ServAddr_f[2];
struct sockaddr_in ClntAddr_f[2];     
unsigned short echoServPort;     /* Server port */
unsigned int clntLen;            /* Length of client address data structure */
char *err_msg;

void DieWithError(char *errorMessage, int I);  /* Error handling function */
void ProcessMain(int port, int I);

void setSocket(int *sock, int port, struct sockaddr_in echoServAddr, int maxPending, int I){
    /* Create socket for incoming connections */
    if ((*sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket", I);
      
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    const int optval = 1;
    setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    char num[10];
    snprintf(num, sizeof(num), "%s(%d)", "bind", I);

    /* Bind to the local address */
    if (bind(*sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind", I);

    /* Mark the socket so it will listen for incoming connections */
    if (listen(*sock, maxPending) < 0)
        DieWithError("listen", I);
}

int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    unsigned int processLimit;       /* Number of child processes to create */
    unsigned int processCt;          /* Process counter */
    unsigned int port[processLimit];
    
    if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Server Port> <Fork limit>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */
    processLimit = atoi(argv[2]);  /* Second arg:  number of child processes */

    for(int i=0; i < processLimit; i++){
        port[i] = echoServPort + i + 1;
        printf("port[%d]: %d\n", i, port[i]); }

    pid_t pid[processLimit];   /* Process ID */
    for(int i=0; i < processLimit; i++){
        pid[i] = fork();
        if(pid[i] == 0){
            ProcessMain(port[i], i);
        }
    }

    setSocket(&servSock, echoServPort, ServAddr, processLimit, 0);

    int num = 10;
    int I=0;
    while(1){
        clntLen = sizeof(ClntAddr);
        if ((clntSock = accept(servSock, (struct sockaddr *) &ClntAddr, 
                                &clntLen)) < 0)
            DieWithError("accept", I);

        if (send(clntSock, &port[I], sizeof(int), 0) != sizeof(int))
            DieWithError("send", I);
        I++;
    }
    for(int i=0; i<processLimit; i++){ wait(&pid[i]); }
    close(clntSock);
    exit(0);
}

void ProcessMain(int port, int I)
{
    int new_sock, new_cli_sock;
    setSocket(&new_sock, port, ServAddr_f[I], 5, I);

    printf("port_child: %d\n", port);
    while(1){
        int clntLenegth = sizeof(ClntAddr_f[I]);
        if ((new_cli_sock = accept(new_sock, (struct sockaddr *) &ClntAddr_f[I], 
                                &clntLenegth)) < 0)
            DieWithError("accept", I);

        int num = 0;
        recv(new_cli_sock, &num, sizeof(int),0);
        printf("\trecv_num: %d\tport: %d\n\n", num, port);
    }
    exit(0);
}
