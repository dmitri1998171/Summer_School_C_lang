#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO1 "/tmp/fifo.1"
#define MAXLINE 80

int main(int argc, char* argv[]){
	int status = 0;
	int readfd, writefd;
	char buff[MAXLINE+1];
	ssize_t n;
    
    //FILE *fp;
    char text[256];
    
//---------------Ввод с клавы в файл-------------------------------
    while(1){
	pid_t pid = fork();
    mkfifo(FIFO1, 0666);
	if(pid == -1){
        printf("Error!\n");
        return 1;
    }
    else if(pid == 0){
        
        //printf("%s: ", argv[1]);
        scanf("%s", text);
        
        strcat(argv[1], ": ");
        strcat(argv[1], text);
        
        writefd = open(FIFO1, O_WRONLY, 0);
        write(writefd, argv[1], sizeof(int));
		close(writefd);
        sleep(1);
        
        return 0;
    }
    
//---------------Вывод на экран из файла-----------------------------
    
    pid_t pid1 = fork();
    if(pid1 == -1){
        printf("Error!\n");
        return 1;
    }
    else if(pid1 == 0){
        readfd = open(FIFO1, O_RDONLY, 0);
        while ((n= read(readfd, buff, sizeof(buff))) > 0){
            printf("%s\n", buff);
        }
        close(readfd);
        
        
        return 0;
    }
//---------------Основной процесс-------------------------------------

    
    wait(&status);
    }
    
    return 0;
}
