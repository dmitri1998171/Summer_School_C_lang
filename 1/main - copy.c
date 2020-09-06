#include <stdio.h>

int main()
{
    int a = 0x00aabb;
    unsigned char c;

    printf("\na=%x\n", a);
    printf("\nc=%d\n", a & (1 << 3));
    
    return 0;
}
