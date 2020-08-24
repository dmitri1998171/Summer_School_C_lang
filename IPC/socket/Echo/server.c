#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 255

int main(){
    int sock, listener;
    struct sockaddr_in addr;
    char buf[N];
    char str[N+10] = "server: ";
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) { perror("socket"); exit(1); }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

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