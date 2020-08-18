#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define ARR_SIZE 255
#define BUFF_SIZE 5    // кол-во байт, считываемых за раз 


char *buf[BUFF_SIZE];
int fdr, fdw, ret;

void renameFunc(char path_r[], char path_w[]){
    char name[255];
    char ext[5];
    
    strcpy(path_w, path_r);
    strcpy(name, strtok(path_w, "."));
    strcpy(ext, strtok(NULL, "."));
    strcat(name, "(1).");
    strcat(name, ext);
    strcpy(path_w, name);
}

void copyFunc(){
    char path_r[ARR_SIZE]={"text.txt"};
    char path_w[ARR_SIZE];
    
    renameFunc(path_r, path_w);
    
    fdr = open(path_r, O_RDONLY);
    fdw = open(path_w, O_CREAT | O_WRONLY, S_IRWXU);

    while((ret = read(fdr, &buf, BUFF_SIZE)) != 0){
        if(ret == -1) perror("Read error");

        write(fdw, &buf, BUFF_SIZE);
    }

    close(fdr);
    close(fdw);
}

int main(){
    
    copyFunc();
        
    return 0;
} 