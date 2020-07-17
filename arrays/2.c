#include <stdio.h>

int main()
{
    int N, b;

    printf("Enter N: ");
    scanf("%d", &N);

    int a[N];

    // создание массива
    for(int i=0; i < N; i++)
    {
        a[i] = i+1;
        printf("%d ", a[i]);
    }

    // разворот массива
    for (int i=0; i<N/2; i++) {
        b = a[i];
        a[i] = a[N-i-1];
        a[N-i-1] = b;
    }

    // вывод на экран
    printf("\n");
    for(int i=0; i < N; i++) printf("%d ", a[i]);

    return 0;
}