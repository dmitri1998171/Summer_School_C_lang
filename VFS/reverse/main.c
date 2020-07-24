#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int fd, ret;
    char value;

    if ((fd = open("text.txt", O_RDONLY))== -1){
        perror("open failed!"); exit(1);}

    while ((ret = read(fd, &value, 1)) > 0){
		putchar(value);
	}

    close(fd);
    return 0;
}