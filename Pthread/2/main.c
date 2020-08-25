#include <pthread.h>
#include <stdio.h>

#define SIZE 1000

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
long A = 0;

void *func(void *param){
    // long *A = (long*) param;

    long tmp;
    for(int i=0; i < 1000000; i++){
    pthread_mutex_lock(&mutex);
        tmp = A;
        tmp++;
        A = tmp;
    pthread_mutex_unlock(&mutex);
    }
    // return NULL;
}

int main(void){
    pthread_t tid[SIZE];

    // pthread_mutex_init(&mutex, NULL);

    for(int i=0; i < SIZE; i++){
        pthread_create(&tid[i], NULL, (void *) func, NULL);
    }

    for(int i=0; i < SIZE; i++){
        pthread_join(tid[i], NULL);
    }

    // pthread_mutex_destroy(&mutex);
    printf("A: %ld\n", A);
    return 0;
}