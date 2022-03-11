#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    int status;
    pid_t pid = fork();
    
    if(pid<0){
        printf("Error!"); 
        return 1;
    }
    else if(pid==0){
        if(execl("./dub", "dublicate.c", argv[1], argv[2], NULL)<0){
            printf("ERROR while start process\n");
            return -2;
        }
        else printf("process started (pid=%d)\n", pid);
        return 0;
        }
    else{
        waitpid(pid, &status, 0);
        return 0;
    }
}
