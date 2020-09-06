#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>


void copyFunc(char *name){
    int h=6, w=50, y = 0, x = 0; 
    WINDOW *mycopywin;
    WINDOW *mysubwin;
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    
    mycopywin = newwin(h, w, (LINES/2)-(h/2), (COLS/2)-(w/2));
    box(mycopywin, 0, 0);

    wattron(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 2, "Copy: ");
    wattroff(mycopywin, COLOR_PAIR(1));
    mvwprintw(mycopywin, h-5, 8, name);

    mysubwin = derwin(mycopywin, 3, w-2, 2, 1);
    box(mysubwin, 0,0);

    wattroff(mycopywin, COLOR_PAIR(1));
    for(int n = 2; n < w-2; n++){
        mvwaddch(mycopywin, h-3, n, '#');
        mvwprintw(mycopywin, h-5, 20, "%d%%", (n*2)+6);
        wrefresh(mycopywin);
        usleep(100000);
    }
    wgetch(mycopywin);
    delwin(mycopywin);
}

int main(){
    initscr();
    
    
    copyFunc("test.txt");
        
    endwin();
    return 0;
} 