#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MSGSZ 128

typedef struct msgbuf{
    long mtype;
    char mtext[MSGSZ];
    int pid;
} message_buf;

pthread_t tid, tid2;
int msqid;
message_buf sbuf;

void *writeFunc(void*);
void *readFunc(void*);

int main(int argc, char *argv[]){
    char str[15];
    int msgflg = IPC_CREAT | 0666;
    key_t key=10;
    // size_t buf_length;

    // создаем очередь
    if ((msqid = msgget(key, msgflg )) < 0){
        perror("msgget"); exit(1); }
    
    while(1){
        // заполняем структуру
        printf("Enter: ");
        // scanf("%s", str);
        gets(str);

        snprintf(sbuf.mtext, sizeof(sbuf.mtext), 
                "%s: %s", argv[1], str);
        
        pthread_create(&tid, NULL, writeFunc, NULL);
        pthread_create(&tid2, NULL, readFunc, NULL);

        pthread_join(tid, NULL);
        pthread_join(tid2, NULL);
    }
    return 0;
}

void *writeFunc(void *param){
    sbuf.mtype = 1;
    size_t buf_length = strlen(sbuf.mtext) + 1;
    sbuf.pid = getpid();

    // отпр. сообщ.
    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0){
        printf ("%d, %ld, %s, %ld\n", 
            msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd"); exit(1); }
    
    // printf("pid: %d\n", getpid());

    // pthread_detach(tid);
    return NULL;
}

void *readFunc(void *param){
    // получ. ответ
    if (msgrcv(msqid, &sbuf, MSGSZ, 1, 0) < 0){
        perror("msgrcv"); exit(1);}

    printf("%s\n", sbuf.mtext);

    return NULL;
}