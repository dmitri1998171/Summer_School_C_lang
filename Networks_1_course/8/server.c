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

int new_key, msgqid, msgqid1;
int qtype = 1;

void checkReg(){
    if((msgqid = msgget(key, IPC_CREAT|0660)) == -1) {
            perror("msgget"); exit(1); }
        
    msgrcv(msgqid, &r_msg, MAX_SEND_SIZE, qtype, 0);
    new_key = r_msg.new_key;
}

int main(int argc, char *argv[]) {
    int  stat;
    struct msg_buf qbuf;
    
    printf("key: %d\n", key);
    checkReg();
    printf("new_key: %d\n", new_key);
    msgctl(msgqid, IPC_RMID, NULL);
    
    if((msgqid1 = msgget(new_key, IPC_CREAT|0660)) == -1) {
            perror("msgget"); exit(1); }
    
    while(1){
		read_message(msgqid1, &qbuf, qtype); 
		sleep(1);
        
        send_message(msgqid1, &qbuf, qtype, qbuf.mtext);
        sleep(1);
    }
    
    msgctl(msgqid1, IPC_RMID, NULL);
    
    return 0;
}
