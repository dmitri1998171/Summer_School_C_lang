#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSZ 128
#define N 15

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
    unsigned int pid;
} message_buf;


int main(){
    int I=0, key_s = 10;
    int msqid[N], msgqid_s;
    int msgflg = IPC_CREAT | 0666;
    int key[N];
    message_buf sbuf;
    size_t buf_length;


    // создаем очередь
    if ((msgqid_s = msgget(key_s, msgflg)) < 0){
        perror("msgget"); exit(1); }

    while (1)
    {   // получ. сообщ.
        if (msgrcv(msgqid_s, &sbuf, MSGSZ, 1, 0) < 0){
            perror("msgrcv"); exit(1); }

        key[I] = sbuf.pid;
        printf("key[%d]: %d\tsbuf.pid: %d\n", I, key[I], sbuf.pid);
        if ((msqid[I] = msgget(key[I], msgflg)) < 0){
        perror("msgget"); exit(1); }
        
        printf("%s\n", sbuf.mtext);
        buf_length = strlen(sbuf.mtext) + 1;
        // printf("key[%d]: %d\tmsqid[%d]: %d\n", I, sbuf.pid, I, msqid[I]);
        
        I++;
        // отпр. ответ
        for(int i=0; i < I; i++){
            if (msgsnd(msqid[i], &sbuf, buf_length, IPC_NOWAIT) < 0){
                printf ("%d, %ld, %s, %ld\n", 
                        msqid[i], sbuf.mtype, sbuf.mtext, buf_length);
                perror("msgsnd"); exit(1); }
        }
        sleep(2);
    }
    return 0;
}