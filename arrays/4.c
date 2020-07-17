#include <stdio.h>

int main()
{
    int i=0, j, k = 0, p=1, N;
    
    printf("Enter N: ");
    scanf("%d", &N);

    int Arr[N][N];
    
    while (i < N*N) /*Цикл по номеру витка*/
    {
        k++;

        // Определение значений верхней строки
        for (j=k-1;j<N-k+1;j++)
        {
            Arr[k-1][j]=p++;
            i++;
        }   
    
        // По правому столбцу
        for (j=k;j<N-k+1;j++)
        {
            Arr[j][N-k]=p++;
            i++;
        }   
    
        // по нижней строке
        for (j=N-k-1;j>=k-1;j--)
        {
            Arr[N-k][j]=p++;
            i++;
        }   
    
        // по левому столбцу
        for (j=N-k-1;j>=k;j--)
        {
            Arr[j][k-1]=p++;
            i++;
        }   
    
    }

    // вывод на экран
    printf("\n");
    for(int i=0; i < N; i++) 
    {
        for(int j=0; j < N; j++)
        {
            printf("%d ", Arr[i][j]);
        }
        printf("\n");
    }
    return 0;
}