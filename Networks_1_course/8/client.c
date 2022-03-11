#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
#include "mymsg.h"

int msgqid, msgqid1;

void regFunc(int new_key, int id){
    struct reg_msg r_msg;
    
    if((msgqid = msgget(key, IPC_CREAT|0660)) == -1) {
            perror("msgget"); exit(1); }
        
    r_msg.mtype = 1;
    r_msg.user_id = id;
    r_msg.new_key = new_key;
    
    msgsnd(msgqid, &r_msg, MAX_SEND_SIZE, 0);
    sleep(1);
    msgctl(msgqid, IPC_RMID, NULL);
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int qtype = 1;
    int  stat;
    struct msg_buf qbuf;
        
    regFunc(atoi(argv[1]), atoi(argv[2]));
    
    if((msgqid1 = msgget(r_msg.new_key, IPC_CREAT|0660)) == -1) {
        perror("msgget"); exit(1); }
    
    while(1){
        // запускаем дочерний процесс 
        pid = fork();

        if (-1 == pid) {
            perror("fork"); // произошла ошибка 
            exit(1); //выход из родительского процесса
        } 
        else if (0 == pid) {
            char str[255];
            scanf("%s", str);
            send_message(msgqid1, (struct msg_buf *)&qbuf, qtype, str);
            exit(0); // выход из процесс-потомока 
        }
    
        read_message(msgqid1, &qbuf, qtype); 
        waitpid(pid, &stat, 0);
        sleep(1);
    }
    msgctl(msgqid1, IPC_RMID, NULL);
    
    return 0;
}
