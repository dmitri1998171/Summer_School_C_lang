#include <pthread.h>        /* for POSIX threads */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>     /* for memset() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include "Variables.h"

int check = 0;

void generate(){				
    srand(time(NULL));

    printf("\nmatrix:\n");
    // генерация матрицы случайных чисел формата 5х5
    for(int i=0; i<msize; ++i){
        for(int j=0; j<msize; ++j){
            A[i][j] = 0 + rand() %9;
            printf("%d ", A[i][j]); // вывод на экран
        }
        printf("\n");
    }

    printf("\nvector:\n");
    //генерация вектора
    for(int j=0; j<msize; ++j){
            vector[j] = 0 + rand() %9;
            printf("%d ", vector[j]); // вывод на экран
        }
    printf("\n\n");
}

void outFunc(char res[msize]){
    printf("\nresult:\n");
    //вывод результата на экран
    for(int j=0; j<msize; ++j){
            printf("%d ", res[j]);
        }
       printf("\n");
}

void DieWithError(char *errorMessage){
    perror(errorMessage);
    exit(1);
}

void ServerSendFunc(int sock)
{
    for(int i=0; i<msize; ++i){
        send(sock, &vector[i], sizeof(int), 0);             // Отправка вектора
    }
    printf("\n");

    //for (int i=0; i<msize; ++i){
        for(int j=0; j<msize; ++j){
            str_m[j] = A[check][j];
            send(sock, &str_m[j], sizeof(int), 0);          // Отправка строки матрицы
        }
    printf("check = %d", check);
    check += 1;
    if(check==msize) check = 0;
}

void ServerRecvFunc(int sock){
    int v[msize];
    int bytesRcvd;

    printf("\nrecv: ");
    for (int i=0; i<msize+1; i++){
        if ((bytesRcvd = recv(sock, &v[i], sizeof(v[i]), 0)) <= 0)
            printf("\n");
        printf("%d ", v[i]);
    }
}


int CreateTCPServerSocket(unsigned short port){

    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
      
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(sock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    return sock;
}


int AcceptTCPConnection(int servSock){
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */

    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);
    
    /* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
           &clntLen)) < 0)
        DieWithError("accept() failed");
    
    /* clntSock is connected to a client! */
    
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    return clntSock;
}