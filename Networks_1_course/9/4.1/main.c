#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
//#include <child.h>

int main(int argc, char* argv[]){
    int status;
    pid_t pid;
    int *shm, *s, shmid, N = 5;
    key_t key = 149;
    int array[5][5];
    
    srand(time(NULL)); // Задаем зерно рандомизации
    
    for(int i=0; i<5; ++i){
            for(int j=0; j<5; ++j){
                array[i][j] = 1 + rand() % 5;// Случайное число в пределе от 1 до 5
                printf("%d ", array[i][j]);
            }
            printf("\n");
        }
        printf("\n");

    
    int size = sizeof(int)*N*N;
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
        	perror("shmget");
        	exit(1);
    	}

	if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        	perror("shmat");
        	exit(1);
    	}

	s = shm;
	int k = 0;
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j) {
			s[k] = array[i][j]; 
			k++;
		}
	}
	
		pid = fork();
        if(pid < 0){
            printf("Error!");
            exit(1);
        }
		if (pid == 0) {
			char KEY[32];
			sprintf(KEY, "%d", key);
			int rc = execl("./child", "child", KEY, NULL);
			if (rc < 0) {
				printf("Error execl!");
			}
			exit(0);
		}
		
    waitpid(pid, &status, 0);
    
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
		printf("Error shmctl!\n");
		exit(1);
	}
	
    return 0;
}
