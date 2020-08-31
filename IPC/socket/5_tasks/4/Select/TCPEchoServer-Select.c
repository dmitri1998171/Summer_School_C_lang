#include "TCPEchoServer.h"  /* TCP echo server includes */
#include <sys/time.h>       /* for struct timeval {} */
#include <fcntl.h>          /* for fcntl() */

int main(int argc, char *argv[])
{
    int *servSock;                   /* Socket descriptors for server */
    int maxDescriptor;               /* Maximum socket descriptor value */
    fd_set sockSet;                  /* Set of socket descriptors for select() */
    long timeout;                    /* Timeout value given on command-line */
    struct timeval selTimeout;       /* Timeout for select() */
    int running = 1;                 /* 1 if server should be running; 0 otherwise */
    int noPorts;                     /* Number of port specified on command-line */
    int port;                        /* Looping variable for ports */
    unsigned short portNo;           /* Actual port number */

    if (argc < 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Timeout (secs.)> <Port 1> ...\n", argv[0]);
        exit(1);
    }

    timeout = atol(argv[1]);        /* First arg: Timeout */
    noPorts = argc - 2;             /* Number of ports is argument count minus 2 */

    servSock = (int *) malloc(noPorts * sizeof(int));
    maxDescriptor = -1;
  
    for (port = 0; port < noPorts; port++)
    {
        portNo = atoi(argv[port + 2]);
        servSock[port] = CreateTCPServerSocket(portNo);

        if (servSock[port] > maxDescriptor)
            maxDescriptor = servSock[port];
    }

    printf("Starting server:  Hit return to shutdown\n");
    while (running)
    {
        FD_ZERO(&sockSet);
        FD_SET(STDIN_FILENO, &sockSet);
        for (port = 0; port < noPorts; port++)
            FD_SET(servSock[port], &sockSet);

        selTimeout.tv_sec = timeout;       /* timeout (secs.) */
        selTimeout.tv_usec = 0;            /* 0 microseconds  */

        if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
            printf("No echo requests for %ld secs...Server still alive\n", timeout);
        else 
        {
            if (FD_ISSET(0, &sockSet)) 
            {
                printf("Shutting down server\n");
                getchar();
                running = 0;
            }

            for (port = 0; port < noPorts; port++)
                if (FD_ISSET(servSock[port], &sockSet))
                {
                    printf("Request on port %d:  ", port);
                    HandleTCPClient(AcceptTCPConnection(servSock[port]));
                }
        }
    }

    for (port = 0; port < noPorts; port++)
        close(servSock[port]);

    free(servSock);
    exit(0);
}
