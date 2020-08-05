#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid1, pid2, pid3, pid4, pid5;
    int status;

    pid1 = fork();
    if(pid1 == 0){
        printf("child 1:\tpid=%d\tppid=%d\n", getpid(), getppid());
        pid5 = fork();
        if(pid5 == 0){
            printf("child 5:\tpid=%d\tppid=%d\n", getpid(), getppid());
            exit(0);
        }
        exit(0);
    }

    pid2 = fork();
    if(pid2 == 0){
        printf("child 2:\tpid=%d\tppid=%d\n", getpid(), getppid());
        pid3 = fork();
        if(pid3 == 0){
            printf("child 3:\tpid=%d\tppid=%d\n", getpid(), getppid());
            exit(0);
        }
        pid4 = fork();
        if(pid4 == 0){
            printf("child 4:\tpid=%d\tppid=%d\n", getpid(), getppid());
            exit(0);
        }
        exit(0);
    }

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);
    waitpid(pid4, &status, 0);
    waitpid(pid5, &status, 0);
    printf("parent1: %d\n", getpid());
    // getchar();
    
    return 0;
}