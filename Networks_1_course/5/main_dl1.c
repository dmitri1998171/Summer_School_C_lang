#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int multiplication(int, int);
int division(int, int);

int main(){
    int x, y, res;
    
    printf("Enter x: ");
    scanf("%d", &x);
    printf("Enter y: ");
    scanf("%d", &y);

    res = multiplication(x, y);
    printf("res: %d\n", res);

    res = division(x, y);
    printf("res: %d\n", res);
    
    return 0;
}
