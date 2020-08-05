#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int status;

    pid = fork();

    if(pid == 0){
        printf("This is a child: %d\n", getpid());
        getchar();
        exit(0);
    }
    else{
        wait(&status);
        printf("This is a parent: %d\n", getpid());
    }
    return 0;
}