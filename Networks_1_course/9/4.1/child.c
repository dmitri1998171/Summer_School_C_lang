#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>

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

//////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
	int *shm, shmid;
	int key = atoi(argv[1]);
    int N=5;

	int mat[N][N];

	int size = sizeof(int)*N*N;
	if ((shmid = shmget(key, size, 0666)) < 0) {
        	perror("shmget");
        	exit(1);
    	}

	if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        	perror("shmat");
        	exit(1);
    	}
	
	int k = 0;
	int *s = shm;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j){
			mat[i][j] = s[k++];
        }
        
    max_len(mat);
    min_len(mat);
    sumFunc(mat);
    divFunc(mat);

    return 0;
}
