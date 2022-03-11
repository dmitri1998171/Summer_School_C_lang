#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>


union semun {
 int val;                  /* значение для SETVAL */
 struct semid_ds *buf;     /* буферы для  IPC_STAT, IPC_SET */
 unsigned short *array;    /* массивы для GETALL, SETALL */
                           /* часть, особенная для Linux: */
 struct seminfo *__buf;    /* буфер для IPC_INFO */
};

int main(int argc, char * argv[]) {
	
	key_t key = 115;
	long *shm;
    const char * path = "./text/";
	int semid, shmid, status;
	union semun arg;
	struct sembuf lock_res = {0, -1, 0};	// Блокировка ресурса
	struct sembuf rel_res = {0, 1, 0};	    // Освобождение ресурса
	
	key = ftok(".", 'S');
	semid = semget(key, 1, 0666 | IPC_CREAT);
	arg.val = 1;
    
	semctl(semid, 0, SETVAL, arg);
	 
	// Создадим область разделяемой памяти
	if ((shmid = shmget(key, sizeof(long), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shm = (long *)shmat(shmid, NULL, 0)) == (long *) -1) {
		perror("shmat");
		exit(1);
	}
	
	(*shm) = 0;

	DIR *d;
  	struct dirent *dir;
  	d = opendir(path);
    
    while ((dir = readdir(d)) != NULL) {
        char fpath[128];
        strcpy(fpath, path);
        strcat(fpath, dir->d_name);
			
        pid_t pid = fork();
        if(pid < 0){
            printf("Error pid!");
            exit(1);
        }
        if (pid == 0) {
            FILE * f = fopen(fpath, "r");
            char c;
            if (f == NULL) exit(0);
            
            while ((c = fgetc(f)) != EOF) {
                if ((shm = (long *)shmat(shmid, NULL, 0)) == (long *) -1) {
                    perror("shmat");
                    exit(1);
                }
					
                if((semop(semid, &lock_res, 1)) == -1){
                    fprintf(stderr, "Lock failed\n");
                    perror(strerror(errno));
                    exit(1);
                } 
					
                (*shm)++;

                if((semop(semid, &rel_res, 1)) == -1){
                    fprintf(stderr, "Unlock failed\n");
                    perror(strerror(errno));
                    exit(1);
                } 
            }

            if (shmdt(shm) < 0) {
                printf("Error shmdt!\n");
                exit(1);
            }

            fclose(f);
            exit(0);
        }
			
        wait(&status);			
    }
    
    closedir(d);
  	
	if ((shm = (long *)shmat(shmid, NULL, 0)) == (long *) -1) {
		perror("shmat");
		exit(1);
	}

	printf("Sum = %ld\n", *shm);
	
	if (shmdt(shm) < 0) {
		printf("Error shmdt!\n");
		exit(1);
	} 
	
	/* Удалим созданные объекты IPC */	
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		printf("Error shmctl!\n");
		exit(1);
    }
	
	if (semctl(semid, 0, IPC_RMID) < 0) {
		printf("Error shmctl!\n");
		exit(1);
	}
	
	return 0;
}
