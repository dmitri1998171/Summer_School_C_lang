#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int fd, ret, i=0;
    char value;

    if ((fd = open("text.txt", O_RDONLY))== -1){
        perror("open failed!"); exit(1);}


    int length = lseek (fd, 0, SEEK_END);

    for(int i = length-1; i >= 0; i--){
        lseek (fd, i, SEEK_SET);
        ret = read(fd, &value, 1);
        putchar(value);
    }

    close(fd);
    return 0;
}