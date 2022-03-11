#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define msize 5
  
int result[msize];

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void calc(int A[msize], int v[msize]){
    printf("result: ");
    // калькуляция произведения матрицы на вектор
    for(int i=0; i<msize; ++i){
        result[i] += A[i]*v[i];
        printf("%d ", result[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    int sock;
    int bytesRcvd;
    char m_buf[255];
    char v_buf[255];
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */

    if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    //echoString = argv[2];         /* Second arg: string to echo */

    if (argc == 3)
        echoServPort = atoi(argv[2]); /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */


    const int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));


    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    int m[msize];
    int v[msize];

    printf("\nvector: ");
    for (int i=0; i<msize; i++){
        if ((bytesRcvd = recv(sock, &v[i], sizeof(int), 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        printf("%d ", v[i]);
    }

    printf("\n");

    printf("matrix: ");
    for (int i=0; i<msize; i++){
        if ((bytesRcvd = recv(sock, &m[i], sizeof(int), 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        printf("%d ", m[i]);
    }

    printf("\n\n");

    calc(m, v);


    for(int i=0; i<msize; ++i){
        send(sock, &result[i], sizeof(int), 0); // Отправка результата
    }

    close(sock);
    exit(0);
}
