#include<stdio.h>	
#include<stdlib.h>	
#include <unistd.h>
#include<string.h>	
#include<netinet/udp.h>	
#include<netinet/ip.h>	
#include<sys/socket.h>
#include<arpa/inet.h>

void ProcessPacket(unsigned char*, int);
void print_udp_packet(unsigned char *, int, int);

int sock_raw;
int udp=1;
struct sockaddr_in source, dest;

int main()
{
	int saddr_size , data_size;
	struct sockaddr saddr;
	struct in_addr in;
	unsigned char *buffer = (unsigned char *)malloc(65536); 
	
	printf("Starting...\n");
	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(sock_raw < 0){ printf("Socket Error\n");	return 1; }

	while(1){
		saddr_size = sizeof(saddr);
		data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, &saddr_size);
		if(data_size < 0){ printf("Recvfrom error , failed to get packets\n"); return 1; }
		
		ProcessPacket(buffer, data_size);
	}
	close(sock_raw);
	return 0;
}

void ProcessPacket(unsigned char* buffer, int size)
{
	struct iphdr *iph = (struct iphdr*)buffer;

    if(iph->protocol == 17) print_udp_packet(buffer, size, udp++);
}

void print_udp_packet(unsigned char *Buffer, int Size, int udp)
{
	unsigned short iphdrlen;
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen = iph->ihl*4;

    memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;
    
	struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen);

	printf("\n\n***********************UDP Packet*************************\tUDP: %d", udp);
	printf("\nIP Header\n");		
    printf("   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
	printf("   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
	printf("\nUDP Header\n");
	printf("   |-Source Port      : %d\n", ntohs(udph->source));
	printf("   |-Destination Port : %d\n", ntohs(udph->dest));
	printf("   |-UDP Length       : %d\n", ntohs(udph->len));
}
