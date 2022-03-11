#ifndef __MYMSG_H__
#define __MYMSG_H__

#include<string.h>
#include<time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/errno.h>

#define MSGPERM 0755    // msg queue permission
#define MSGTXTLEN 256   // msg text length
#define MAX_SEND_SIZE 255

int key = 150;

struct msg_buf {
  long mtype;
  char mtext[MSGTXTLEN];
} msg;

struct reg_msg {
  long mtype;
  int user_id;
  int new_key;
} r_msg;

void send_message(int qid, struct msg_buf *qbuf, long type, char *text){
        qbuf->mtype = type;
        strcpy(qbuf->mtext, text);
        printf("id:%d Send:%s type:%ld len:%ld\n",qid,qbuf->mtext,qbuf->mtype,strlen(qbuf->mtext)+1);

        if((msgsnd(qid, (struct msgbuf *)qbuf, strlen(qbuf->mtext)+1, IPC_NOWAIT)) ==-1){
                perror("msgsnd");
                exit(1);
        }
}

void read_message(int qid, struct msg_buf *qbuf, long type){
        qbuf->mtype = type;
        msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
        printf("Text:%s\n", qbuf->mtext);
}

#endif
