#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void bubbleSort(int *num, int size)
{
  // Для всех элементов
  for (int i = 0; i < size - 1; i++)
  {
    for (int j = (size - 1); j > i; j--) // для всех элементов после i-ого
    {
      if (num[j - 1] > num[j]) // если текущий элемент меньше предыдущего
      {
        int temp = num[j - 1]; // меняем их местами
        num[j - 1] = num[j];
        num[j] = temp;
      }
    }
  }
}

int main(int argc, char* argv[]){
    int x=atoi(argv[1]);
    
    printf("\nsubprogramm is run!\n");
    for(int i=0; i<argc; ++i){
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    
    if(x==1){
        int n=atoi(argv[2]);
        int array[n][n];
        
        for(int i=0; i<n; ++i){
            for(int j=0; j<n; ++j){
                array[i][j] = 0;
                printf("%d ", array[i][j]);
            }
            printf("\n");
        }
    }
    else if(x==2){
        printf("%s\n", argv[2]);
    }
    else if(x==3){
        
        int len=strlen(argv[2]);
        int array[len];
        int n=atoi(argv[2]);
        
        for(int i = len-1; n; n/=10) array[i--] = n%10;//split a number in to array
        
        //sort
        bubbleSort(array, len);
        
        //output
        for(int i=0; i<len; ++i){
            printf("%d", array[i]);
        }
        printf("\n");
    }
    else{ 
        printf("Error! The arguments may be 1,2 or 3 only!"); 
    }
    return 0;
}
