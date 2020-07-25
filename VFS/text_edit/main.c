#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int row=0;
int col=0;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void read_text(int fd){
    char c;
    int length = lseek(fd, 0, SEEK_END);
    lseek (fd, 0, SEEK_SET);

    for(int i=0; i<length; i++){
        read(fd, &c, 1);
        // putchar(c);
        addch(c);
    }
}

void cur_control()
{
    int c = getch();
    switch(c){
        case KEY_LEFT: if (col > 0) col--; break;
        case KEY_RIGHT: col++; break;
        case KEY_UP: if (row > 0) row--; break;
        case KEY_DOWN: row++; break;
        default: break;
    }
}

int main(int argc, char *argv[]){
    int fd;
    char str;

    initscr();
    signal(SIGWINCH, sig_winch);
    // cbreak ();
    // nonl ();
    // noecho ();
    // idlok (stdscr, TRUE);
    keypad (stdscr, TRUE);
    
    if ((fd = open(argv[1], O_RDONLY))== -1){
        perror("open failed!"); exit(1);}

    /* Читаем файл */
    read_text(fd);
    move(0,5);

    while(true){
        move(row, col);
        refresh();
        cur_control();
    }

    close(fd);
    endwin();
    return 0;
}