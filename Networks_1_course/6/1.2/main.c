#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]) {
        int status;
        
        pid_t pid = fork();
        if(pid<0){
            printf("Error!");
        }
        else if(pid==0){//A
            pid_t pid1 = fork();
            if(pid1<0){
                printf("Error!");
            }
            else if(pid1==0){//В
                printf("pid1.1:%d\n",getpid());
                return 0;
            }
            pid_t pid1_1 = fork();
            if(pid1<0){
                printf("Error!");
            }
            else if(pid1_1==0){//Г
                printf("pid1.2:%d\n",getpid());
                return 0;
            }
            
            pid_t pid1_2 = fork();
            if(pid1<0){
                printf("Error!");
            }
            else if(pid1_2==0){//Д
                printf("pid1.3:%d\n",getpid());
                return 0;
            }
            else{
                waitpid(pid1, &status, 0);
                return 0;
            }
        }
        
        else if(pid==0){//Б
            pid_t pid2 = fork();
            if(pid2<0){
                printf("Error!");
                return 0;
            }
            else if(pid2==0){//Ж
               printf("pid2.1:%d\n",getpid());
               return 0;
            }
            pid_t pid2_1 = fork();
            else if(pid2==0){//Д
                printf("pid2.2:%d\n",getpid());
                return 0;
            }
            else{
                waitpid(pid2, &status, 0);
                return 0;
            }
        }
        else{//main
            waitpid(pid, &status, 0);
            return 0;
        }
}
