#include <pthread.h>
#include <stdio.h>

#define SIZE 1000

long A = 0;

void* func(void *param){
    long tmp;
    for(int i=0; i < 1000000; i++){
        tmp = A;
        tmp++;
        A = tmp;
    }
}

int main(void){
    pthread_t tid[SIZE];

    for(int i=1; i < SIZE; i++){
        pthread_create(&tid[i-1], NULL, func, NULL);
    }

    for(int i=0; i < 5; i++){
        pthread_join(tid[i], NULL);
    }

    printf("A: %ld\n", A);

    return 0;
}