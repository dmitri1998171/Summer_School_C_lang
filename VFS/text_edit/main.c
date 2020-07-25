#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <panel.h>
#include <curses.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int row=0;
int col=0;
int I=0, line=0;
char c;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void read_text(int fd){
    int length = lseek(fd, 0, SEEK_END);
    lseek (fd, 0, SEEK_SET);

    // for(int i=0; i<length; i++){
        while(true){
        if(read(fd, &c, 1) == 1) {
            addch(c);
            if(c == '\n') line++;
        }
        else break;
        I++;
        // refresh();
    }
}

// void write_func(){
//     int l,n,i;
//     for (l = 0; l < LINES - 1; l++) {
//         n = len(l);
//         for (i = 0; i < n; i++)
//             putc (mvinch (l, i) & A_CHARTEXT, fd);
//         putc('\n', fd);
//     }
// }

void write_func(int **fd){
    for(int i=0; i<I; i++){
        write(**fd, &c, 1);
    }
}

void keyboard(int *fd)
{
    int c = getch();
    switch(c){
        case KEY_LEFT: if (col > 0) col--; break;
        case KEY_RIGHT: col++; break;
        case KEY_UP: if (row > 0) row--; break;
        case KEY_DOWN: row++; break;

        case KEY_F(1): write_func(&fd); break;
        case 27:        // ESCAPE
            close(*fd);
            endwin();
            exit(0);

        default: 
            insch(c);
            move(row, col++);
            refresh();
            break;
    }
}

int main(int argc, char *argv[]){
    int fd;
    char str;

    initscr();
    signal(SIGWINCH, sig_winch);
    // cbreak ();
    // nonl ();
    // idlok (stdscr, TRUE);
    noecho ();
    keypad (stdscr, TRUE);
    
    if ((fd = open(argv[1], O_RDONLY))== -1){
        perror("open failed!"); exit(1);}

    /* Читаем файл */
    read_text(fd);
    move(0,5);

    while(true){
        move(row, col);
        refresh();
        keyboard(&fd);
    }

    close(fd);
    endwin();

    return 0;
}