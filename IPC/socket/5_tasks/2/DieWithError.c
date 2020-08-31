#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */

void DieWithError(char *errorMessage, int I)
{
    char num[50];
    snprintf(num, sizeof(num), "%s(%d)", errorMessage, I);
    perror(num);
    exit(1);
}
