#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <panel.h>
#include <curses.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_NAME_LEN 44

int row=0, col=0;
int I=0, line=0;
int fd, win_w, win_h;
int newwin_x, newwin_y, newwin_w = MAX_NAME_LEN, newwin_h = 1;
char c;

void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
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

void read_text(int fd){
    while(true){
        if(read(fd, &c, 1) == 1){
            addch(c);
            if(c == '\n') line++;
        }
        else break;
        I++;
    }
}

void open_func(){
    WINDOW * wnd;
    char filename[MAX_NAME_LEN+1];
    
    newwin_y = (win_h/2)-(newwin_h/2);
    newwin_x = (win_w/2)-(newwin_w/2);

    start_color();
    refresh();
    echo();
    init_pair(1, COLOR_YELLOW, COLOR_BLUE);     
    wnd = newwin(newwin_h, newwin_w, newwin_y, newwin_x);
    wbkgd(wnd, COLOR_PAIR(1));      
    wattron(wnd, A_BOLD);
    wprintw(wnd, "Filename: ");
    wgetnstr(wnd, filename, MAX_NAME_LEN);
    filename[MAX_NAME_LEN] = 0;
    wrefresh(wnd);
    delwin(wnd);
    refresh();
    
    if((fd = open(filename, O_RDWR))== -1){
        endwin(); 
        perror("open error"); 
        exit(1);
        }

    /* Читаем файл */
    read_text(fd);
}

void write_func(){
    for(int i=0; i<I; i++){
        write(fd, &c, 1);
    }
}

void keyboard()
{
    noecho();                       // отключает автоматическое отображение при вводе
    int c = getch();
    switch(c){
        case KEY_LEFT: if (col > 0) col--; break;
        case KEY_RIGHT: col++; break;
        case KEY_UP: if (row > 0) row--; break;
        case KEY_DOWN: row++; break;

        case KEY_F(1): open_func(); break;
        case KEY_F(2): write_func(); break;
        case 27:        // ESCAPE
            close(fd);
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
    initscr();
    signal(SIGWINCH, sig_winch);
    // cbreak();
    // nonl ();
    // idlok (stdscr, TRUE);
    keypad(stdscr, TRUE);           // вкл. поддержку функциональных клавиш
    
    win_w = getmaxx(stdscr);        // ширина экрана
    win_h = getmaxy(stdscr);        // высота 
    
    mvprintw(19, 3, "F1 - Open  F2 - Save  ESC - exit; w=%d h=%d", win_w, win_h);

    while(true){
        move(row, col);             // Перемещение курсора
        refresh();                  // Обновить
        keyboard(&fd);
    }

    close(fd);
    endwin();

    return 0;
}

/*
    TODO

1. Запись в файл
\/ 2. панель с кнопками
3. удалить окно открытия файла

*/