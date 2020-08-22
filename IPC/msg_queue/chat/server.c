#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
    int pid;
} message_buf;


int main(){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key=10;
    message_buf sbuf;
    size_t buf_length;

    int A[15];
    int I=0;

    // создаем очередь
    if ((msqid = msgget(key, msgflg )) < 0){
        perror("msgget"); exit(1); }

    while (1)
    {   // получ. сообщ.
        if (msgrcv(msqid, &sbuf, MSGSZ, 1, 0) < 0){
            perror("msgrcv"); exit(1); }

        A[I] = sbuf.pid;
        I++;
        
        printf("%s\n", sbuf.mtext);
        buf_length = strlen(sbuf.mtext) + 1 ;
        
        // отпр. ответ
        for(int i=0; i < 2; i++){
            if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0){
                printf ("%d, %ld, %s, %ld\n", 
                        msqid, sbuf.mtype, sbuf.mtext, buf_length);
                perror("msgsnd"); exit(1); }
        }
        sleep(2);
    }
    return 0;
}