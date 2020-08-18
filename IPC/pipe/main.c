#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <malloc.h>

void splitFunc(char str[], char *sep, char c1[]){
    char *t1;
    int i = 0;

    t1 = strtok(str, "|");
    while (t1 != NULL) {
        strcpy(&c1[i], t1);
        t1 = strtok(NULL, "|");
        i++;
    }
}

int main(){
    char str[255];   // исходная строка
    char c1[15][15]; // команды
    char c2[15][15]; // аргументы 1-ой команды
    char c3[15][15]; // аргументы 2-ой команды
    char *t1, *t2, *t3;
    int i=0, j=0, k=0;
    int stat;
    int mypipe[2];
    pid_t pid1, pid2;

    printf("\n$: ");
    gets(str);    

    // разбиваем строку на команды
    t1 = strtok(str, "|");
    while (t1 != NULL) {
        strcpy(c1[i], t1);
        t1 = strtok(NULL, "|");
        i++;
    }
    
    // splitFunc(str, "|", c1);

    // разбиваем 1-ую команду на аргументы
    t2 = strtok(c1[0], " ");
    while(t2 != NULL){
        strcpy(c2[j], t2);
        t2 = strtok(NULL, " ");
        j++;
    }

    // разбиваем 2-ую команду на аргументы
    t3 = strtok(c1[1], " ");
    while(t3 != NULL){
        // printf("%s\n", t3);
        strcpy(c3[k], t3);
        t3 = strtok(NULL, " ");
        k++;
    }

    printf("\n");
    
    // создаем канал
    if (pipe (mypipe)){
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;}

    pid1 = fork();

    if(pid1 == 0){            // команда 1
        close(mypipe[0]);
        dup2(mypipe[1], 1);     // STDOUT в pipe

        execlp(c2[0], c2[0], c2[1], NULL);
        exit(0);
    }

    pid2 = fork();
    if(pid2 == 0){            // команда 2
        close(mypipe[1]);
        dup2(mypipe[0], 0);     // pipe в STDIN

        execlp(c3[0], c3[0], c3[1], NULL);
        exit(0);
    }

    waitpid(pid1, &stat, 0);
    waitpid(pid2, &stat, 0);

    return 0;
}