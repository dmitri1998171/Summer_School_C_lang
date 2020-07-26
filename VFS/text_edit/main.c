#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <panel.h>
#include <curses.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_NAME_LEN 44

WINDOW * btmwnd;
int row=0, col=0;
int I=0, line=0;
int fd, win_w, win_h;
int newwin_x, newwin_y, newwin_w = MAX_NAME_LEN, newwin_h = 1;
char filename[MAX_NAME_LEN+1];

void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

int len(int lineno){
    int linelen = COLS - 1;
    while (linelen >= 0 && mvinch (lineno, linelen) == ' ')
    linelen--;
    return linelen + 1;
}

void read_func(){
    char c;
    while(1){
        if(read(fd, &c, 1) == 1){
            addch(c);
            if(c == '\n') line++;
        }
        else break;
        I++;
    }
}

void write_func(){
    int length;
    char c;

    for(int i=0; i<LINES-1; i++){
        length = len(i);
        for(int j=0; j<length; j++){
            c = mvinch(i, j) & A_CHARTEXT;
            write(fd, &c, 1);
        }
        write(fd, "\n", 1);
    }
}

void open_func(int flag){
    if(flag == O_WRONLY){
        flag = O_TRUNC | O_WRONLY;      // очищаем и перезаписываем файл
        if((fd = open(filename, flag))== -1){
        endwin(); perror("open error"); exit(1);}
        write_func();}

    if(flag == O_RDONLY){               // читаем файл
        if((fd = open(filename, flag))== -1){
        endwin(); perror("open error"); exit(1);}
        read_func();}

    close(fd);
}

void bottom_menu_func(){
    start_color();
    refresh();
    echo();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    btmwnd = newwin(1, win_w, win_h-2, 0);
    wbkgd(btmwnd, COLOR_PAIR(1));
    wprintw(btmwnd, "   F1 - Open  F2 - Save  ESC - exit\t");
    wrefresh(btmwnd);
}

void open_Button(){
    WINDOW * wnd;
    
    newwin_y = (win_h/2)-(newwin_h/2);      // рассчет x,y; коррекция по центру экрана
    newwin_x = (win_w/2)-(newwin_w/2);

    start_color();
    refresh();
    echo();
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);     
    wnd = newwin(newwin_h, newwin_w, newwin_y, newwin_x);
    wbkgd(wnd, COLOR_PAIR(2));      
    wattron(wnd, A_BOLD);
    wprintw(wnd, "Filename: ");
    wgetnstr(wnd, filename, MAX_NAME_LEN);
    filename[MAX_NAME_LEN] = 0;
    wrefresh(wnd);
    delwin(wnd);
    clear();            // Полная очистка экрана
    
    bottom_menu_func();
    open_func(O_RDONLY);
}

void keyboard(){
    noecho();     // откл. авто отображение при вводе
    int c = getch();
    switch(c){
        case KEY_LEFT: if (col > 0) col--; break;
        case KEY_RIGHT: col++; break;
        case KEY_UP: if (row > 0) row--; break;
        case KEY_DOWN: row++; break;

        case KEY_F(1): open_Button(); break;
        case KEY_F(2): open_func(O_WRONLY); break;
        case 27:        // ESCAPE
            close(fd);
            delwin(btmwnd);
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
    keypad(stdscr, TRUE);           // вкл. поддержку функциональных клавиш
    
    win_w = getmaxx(stdscr);        // ширина экрана
    win_h = getmaxy(stdscr);        // высота 
    
    bottom_menu_func();             // отрисовка кнопок F1, F2, ESC
    
    while(1){
        move(row, col);             // Перемещение курсора
        refresh();                  // Обновить
        keyboard();
    }

    delwin(btmwnd);
    endwin();
    return 0;
}

/*
    TODO

\/ 1. Запись в файл
\/ 2. панель с кнопками
\/ 3. удалить окно открытия файла

*/