#include <stdio.h>

int main()
{
    int a = 270533154;
    int size;
    char *ptr;

    ptr = (char*) &a;

    size = sizeof(a);
    printf("size of a: %d\n\n", size);

    // printf("1st byte of a = %d\n", *ptr);
    // ptr++;
    // printf("2nd byte of a = %d\n", *ptr);
    // ptr++;
    // printf("3rd byte of a = %d\n", *ptr);
    // ptr++;
    // printf("4th byte of a = %d\n", *ptr);

    for(int i=0; i < size; i++)
    {
        printf("%d\n", *ptr);
        ptr++;
    }

    return 0;
}