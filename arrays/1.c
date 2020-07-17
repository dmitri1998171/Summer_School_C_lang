#include <stdio.h>

int main()
{
    int N;

    printf("Enter N: ");
    scanf("%d", &N);

    int a[N];

    for(int i=1; i < N+1; i++)
    {
        a[i] = i;
        printf("%d ", a[i]);

        if(i%3==0) printf("\n");
    }

    return 0;
}