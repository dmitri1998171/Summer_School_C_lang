#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<netinet/udp.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

char msg1[] = "Hello there!\n";
char msg2[] = "Bye bye!\n";

int main()
{
    int sock;
    unsigned short iphdrlen;
    unsigned char *Buffer = (unsigned char *)malloc(65536); 
    struct sockaddr_in addr;
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sendto(sock, msg1, sizeof(msg1), 0,
           (struct sockaddr *)&addr, sizeof(addr));

    printf("   |-Source Port      : %d\n", ntohs(udph->source));
	printf("   |-Destination Port : %d\n", ntohs(udph->dest));
	printf("   |-UDP Length       : %d\n", ntohs(udph->len));

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    send(sock, msg2, sizeof(msg2), 0);

    close(sock);

    return 0;
}