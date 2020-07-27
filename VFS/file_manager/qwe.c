#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

int main() {
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, NULL, alphasort);
    
    if (n < 0) perror("scandir");
    // printf("%d\n", n);

    for(int i=0; i<n; i++){
        printf("%s\n", namelist[i]->d_name);
        free(namelist[i]);
    }
    free(namelist);
};