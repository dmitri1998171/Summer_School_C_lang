#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


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

//===========================================================

void *reversit(void *args){
    char *s = (char*)args;
    //printf("s: %s\n", s);
    
    // разворот слова в обратном порядке
    my_strrev(s);
    
    printf("%s\n", s);
    return 0;
}

int main(int argc, char** argv){
    char s[255];
    int i=0;
    void *status;
    
    printf("Enter a string: ");
    fgets(s, 255, stdin);
    
	pthread_t * threads = NULL;
    char *str = strtok(s, " ");
      while (str != NULL) // пока есть лексемы
    {
        threads = realloc(threads, (i + 1) * sizeof(pthread_t));
		pthread_create(&threads[i], NULL, reversit, str);
		str = strtok(NULL, " ");
        i++;
        sleep(1);
	}
	
	for(int j=0; j<i; ++j) pthread_join(threads[j], &status);
	
    return 0;
}

/*
 \/ 1) кол-во слов
 
 \/ 2) разделение строки на слова
 
 \/ 3) переворот слов наоборот
 
  \/ 4) потоки
 */
