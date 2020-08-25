#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 355

int main(int argc, char *argv[]){
    char *servIP;                   
    char *echoString;
    char buf[N];
    char str[N+20] = "server: ";
    unsigned short echoServPort;
    int sock, listener;
    int bytes_read;
    struct sockaddr_in addr;

    if ((argc < 3) || (argc > 4)) {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Port>\n",
               argv[0]);
       exit(1);}

    servIP = argv[1];             
    echoServPort = atoi(argv[2]);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) { perror("socket"); exit(1); }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(echoServPort);
    addr.sin_addr.s_addr = inet_addr(servIP);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        { perror("bind"); exit(2); }

    listen(listener, 1);
    
    while(1){
        sock = accept(listener, NULL, NULL);
        if(sock < 0) { perror("accept"); exit(3); }

        while(1){
            bytes_read = recv(sock, buf, N, 0);
            if(bytes_read <= 0) break;
            
            strcat(str, buf);
            send(sock, str, sizeof(str), 0);
        }
        close(sock);
    }
    return 0;
}