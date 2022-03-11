#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n=5;

int array[5][5];

int max_len(int array[5][5]){
    int max=array[0][0];
    
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            if(array[i][j]>max) max = array[i][j];
        }
    }
    printf("max: %d\n", max);
    return 0;
}

int min_len(int array[5][5]){
    int min=array[0][0];
    
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            if(array[i][j]<min) min = array[i][j];
        }
    }
    printf("min: %d\n", min);
    return 0;
}

int sumFunc(int array[5][5]){
    int sum=0;
    
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            sum += array[i][j];
        }
    }
    printf("sum: %d\n", sum);
    return 0;
}

int divFunc(int array[5][5]){
    int div=1;
    
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            div *= array[i][j];
        }
    }
    printf("div: %d\n", div);
    return 0;
}


////////////////////////////////////////////////////////////


int main(int argc, char* argv[]){
    int fd[2];
    int status;
    char buf[80];
    
    srand(time(NULL)); // Задаем зерно рандомизации
    
    for(int i=0; i<5; ++i){
            for(int j=0; j<5; ++j){
                array[i][j] = 1 + rand() % 5;// Случайное число в пределе от 1 до 5
                printf("%d ", array[i][j]);
            }
            printf("\n");
        }
        printf("\n");


    pipe(fd);
    pid_t pid = fork();
    
    
    if(pid<0){
        printf("Error!"); 
        return 1;
    }
    else if(pid==0){// MAX
        close(fd[0]);
        max_len(array);
        write(fd[1], array, sizeof(int));
        return 0;
        }
    pid_t pid1 = fork();
    if(pid1<0){
        printf("Error!"); 
        return 1;
    }
    else if(pid1==0){// MIN
        close(fd[0]);
        min_len(array);
        write(fd[1], array, sizeof(int));
        return 0;
        }
    pid_t pid2 = fork();
    if(pid2<0){
        printf("Error!"); 
        return 1;
    }
    else if(pid2==0){// SUM
        close(fd[0]);
        sumFunc(array);
        write(fd[1], array, sizeof(int));
        return 0;
        }
    pid_t pid3 = fork();
    if(pid3<0){
        printf("Error!"); 
        return 1;
    }
    else if(pid3==0){// DIV
        close(fd[0]);
        divFunc(array);
        write(fd[1], array, sizeof(int));
        return 0;
        }
    else{
        close(fd[1]);
        
        read(fd[0], &buf, sizeof(buf));
        
        waitpid(pid, &status, 0);
        waitpid(pid1, &status, 0);
        waitpid(pid2, &status, 0);
        waitpid(pid3, &status, 0);
        return 0;
    }
}
