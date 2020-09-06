#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int arr[5];
int cust_arr = 1000;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *loadFunc(void *param){
    for(int i=0; i<5; i++){
        if(arr[i] == 0) arr[i] += 500;
        printf("arr[%d]: %d\n", i, arr[i]);
        for(int j=0; j<5; j++){
            printf("arr[%d]: %d ", j, arr[j]); }
        sleep(1);
    }
    return NULL;
}

void *custFunc(void *param){
    int *i = (int*) param;
    
    printf("cust[%d]: %d\n", *i, cust_arr);
    pthread_mutex_lock(&mutex);
    cust_arr += arr[*i];
    arr[*i] = 0;
    printf("cust[%d]: %d\n", *i, cust_arr);
    pthread_mutex_unlock(&mutex);

    sleep(2);
    return NULL;
}

int main(){
    pthread_t tid[4];

    srand(time(0));

    for(int i=0; i<5; i++){
        pthread_mutex_lock(&mutex);
        arr[i] = rand()%100 + 450;
        pthread_mutex_unlock(&mutex);
    }

    pthread_create(&tid[0], NULL, loadFunc, NULL);
    for(int i=1; i<=3; i++){
        pthread_create(&tid[i], NULL, custFunc, &i); }

    pthread_join(tid[0], NULL);
    for(int i=1; i<=3; i++){
        pthread_join(tid[i], NULL);}

    printf("\n");
    return 0;
}