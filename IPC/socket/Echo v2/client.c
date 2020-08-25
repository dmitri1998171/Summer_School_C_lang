#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char message[] = "Hello there!\n";
char buf[300];

int main(int argc, char *argv[]){
    char *servIP;                   
    char *echoString;
    unsigned short echoServPort;
    int sock;
    struct sockaddr_in addr;

    if ((argc < 3) || (argc > 4)) {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Port> <Echo Word>\n",
               argv[0]);
       exit(1);}

    servIP = argv[1];             
    echoServPort = atoi(argv[2]);
    echoString = argv[3];        

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) { perror("socket"); exit(1); }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(echoServPort); 
    addr.sin_addr.s_addr = inet_addr(servIP);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        { perror("connect"); exit(2); }

    send(sock, echoString, sizeof(echoString), 0);
    recv(sock, buf, sizeof(buf), 0);
    
    printf("%s\n", buf);
    close(sock);

    return 0;
}