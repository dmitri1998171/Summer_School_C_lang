#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSZ 128

typedef struct msgbuf{
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key=10;
    message_buf sbuf;
    size_t buf_length;

    // создаем очередь
    if ((msqid = msgget(key, msgflg )) < 0){
        perror("msgget");
        exit(1); }

    // заполняем структуру
    sbuf.mtype = 1;
    (void) strcpy(sbuf.mtext, "Hello");
    buf_length = strlen(sbuf.mtext) + 1;
    
    // отпр. сообщ.
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0){
       printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1); }

    // получ. ответ
    if (msgrcv(msqid, &sbuf, MSGSZ, 1, 0) < 0){
        perror("msgrcv");
        exit(1);}

    printf("%s\n", sbuf.mtext);
      
    return 0;
}