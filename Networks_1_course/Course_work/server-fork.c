#include <sys/wait.h>
#include "TCPEchoServer.h"
#include "protocol.h"

void TCP1(int);        
void TCP2(int); 

void UDP1(int msqid){
    udpServsock = CreateUDPSocket(UDPPORT1);

    for (;;)  /* Run forever */
    {
            // get value msg_qnum
        if (msgctl(msqid, IPC_STAT, &q) < 0) DieWithError("msgctl() failed");

        qmsgSize = q.msg_qnum;
        
        if(qmsgSize < MAX_LEN_MSG){     
            if(sendto(udpServsock, &qmsgSize, sizeof(qmsgSize), 0, (struct sockaddr *) &UDPechoServAddr, sizeof(UDPechoServAddr)) < 0)
                DieWithError("UDPsend() sent a different number of bytes than expected\n");
            
            printf("\tUDP1: qmsgSize=%d sleep=%d\n", qmsgSize, K);
            sleep(K);
        }
    }
}

void UDP2(int msqid){
    udpServsock = CreateUDPSocket(UDPPORT2);

    for (;;)  /* Run forever */
    {
            // get value msg_qnum
        if (msgctl(msqid, IPC_STAT, &q) < 0) DieWithError("msgctl() failed");

        qmsgSize = q.msg_qnum;
        
        if(qmsgSize > 0){     
            if(sendto(udpServsock, &qmsgSize, sizeof(qmsgSize), 0, (struct sockaddr *) &UDPechoServAddr, sizeof(UDPechoServAddr)) < 0)
                DieWithError("UDPsend() sent a different number of bytes than expected\n");
            
            printf("\tUDP2: qmsgSize=%d sleep=%d\n", qmsgSize, L);
            sleep(L);
        }
    }
}

int main(int argc, char *argv[])
{
    int status;
    pid_t pid1, pid2, pid3, pid4;              
    
    int key;
    int msqid;
    if ((key = ftok(".", 'S')) < 0) DieWithError("ftok() failed");
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {DieWithError("msgget() failed");}

    if ((pid1 = fork()) < 0) DieWithError("fork() failed");
    else if (pid1 == 0) {
            TCP1(msqid);
            return 0;
    }

    if ((pid2 = fork()) < 0) DieWithError("fork() failed");
    else if (pid2 == 0) {
            TCP2(msqid);
            return 0;
    }

    if ((pid3 = fork()) < 0) DieWithError("fork() failed");
    else if (pid3 == 0) { 
            UDP1(msqid);
            return 0;
    }

    if ((pid4 = fork()) < 0) DieWithError("fork() failed");
    else if (pid4 == 0) {
            UDP2(msqid);
            return 0;
    }

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
            
        // Delete queue 
    if (msgctl(msqid, IPC_RMID, 0) < 0) DieWithError("msgctl() failed");

    waitpid(pid3, &status, 0);
    waitpid(pid4, &status, 0);
    

    exit(0);  
}

void TCP1(int msqid)
{
    printf("TCP1 running!\n");
    struct protocol proto;
    int clntSock;                  
    int servSock = CreateTCPSocket(TCPPORT1);

    if (bind(servSock, (struct sockaddr *) &TCPechoServAddr, sizeof(TCPechoServAddr)) < 0)
        DieWithError("TCP1bind() failed");

    if (listen(servSock, MAXPENDING) < 0) DieWithError("TCPlisten() failed");

    for (;;)  /* Run forever */
    {
        clntSock = AcceptTCPConnection(servSock);
        
            // get value msg_qnum
        if (msgctl(msqid, IPC_STAT, &q) < 0) DieWithError("msgctl() failed");


        if(q.msg_qnum < MAX_LEN_MSG){
            if (recv(clntSock, &proto, sizeof(proto), 0) < 0)
                DieWithError("TCPrecv() failed");
            
            //proto.text[proto.len] = '\0';
            printf("TCP: time=%d len=%d text=%s\n", proto.time, proto.len, proto.text);
                
                // send message to queue
            if (msgsnd(msqid, &proto, sizeof(proto) - sizeof(long), 0) < 0)    
                DieWithError("msgsnd() failed\n");

        }
        close(clntSock);
    }
    close(servSock);
}

void TCP2(int msqid)
{
    printf("TCP2 running!\n");
    struct protocol proto;
    int clientSock;                  
    int servSock = CreateTCP2Socket(TCPPORT2);

    if (bind(servSock, (struct sockaddr *) &TCP2echoServAddr, sizeof(TCP2echoServAddr)) < 0)
        DieWithError("TCP2bind() failed");

    if (listen(servSock, MAXPENDING) < 0) DieWithError("TCP2listen() failed");

    for (;;)  /* Run forever */
    {
        clientSock = AcceptTCPConnection(servSock);

            // get value msg_qnum
        if (msgctl(msqid, IPC_STAT, &q) < 0) DieWithError("msgctl() failed");

        if(q.msg_qnum > 0){
                // received message from queue 
            if (msgrcv(msqid, &proto, sizeof(proto) - sizeof(long), 0, 0) < 0) DieWithError("msgrcv() failed");

            if (send(clientSock, &proto, sizeof(proto), 0) != sizeof(proto))
                DieWithError("TCP2send() sent a different number of bytes than expected");

            proto.text[proto.len] = '\0';
            printf("TCP2: time=%d len=%d text=%s\n", proto.time, proto.len, proto.text);

        }
        close(clientSock);   
    }
    close(servSock);
}
