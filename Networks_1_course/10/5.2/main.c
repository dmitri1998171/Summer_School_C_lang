#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

struct {
	pthread_mutex_t mutex;
	FILE * f; 

} shared;

char * my_strrev(char s[]){
    int len = strlen(s);
    // разворот слова в обратном порядке
    for(int j = 0;j <len/2;j++) {
        char temp = s [j]; 
        s [j] = s [len-j-1]; 
        s [len-j-1] = temp;
    }
    return s;
}

void * reversit(void * args) {
	int *id = (int*)args;
    
	while (1) { 
		pthread_mutex_lock(&shared.mutex);
		char buff[100];
		char * res = fgets(my_strrev(buff), 100, shared.f);
		pthread_mutex_unlock(&shared.mutex);
        if (res == NULL)
			return 0;
        
		printf("id: %d\treverse: %s", *id, res);
	}
	return 0;
}


int main(int argc, char * argv[]) {
	FILE * f = fopen(argv[1], "r");
    int N = 3;
    
	shared.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	shared.f = f;

	pthread_t threads[N];
	for (int i = 0; i < N; ++i)
		pthread_create(&threads[i], NULL, reversit, &i);

	for (int i = 0; i < N; ++i)
		pthread_join(threads[i], NULL);

	fclose(f);		
	
	return 0;
}



