#include <stdio.h>
#include <stdlib.h>

#define N 3

int main()
{
    int a[N][N];

    for(int i=0; i < N; i++)
    {
        for(int j=0; j < N; j++)
        {
            a[i][j] = 0;
            if(j > N-i-2) a[i][j] = 1;
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }

    return 0;
}