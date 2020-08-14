#include <stdio.h>
#include <unistd.h>
#include <fork.h>

int main(){
    pid_t pid1, pid2;
    char str[255], c1[255], c2[255];

    printf("$: ");
    scanf("%s", str);

    strcpy(c1, strtok(str, "|"));
    strcpy(c2, strtok(NULL, "|"));

    printf("c1: %s\tc2: %s\n", c1, c2);

    return 0;
}