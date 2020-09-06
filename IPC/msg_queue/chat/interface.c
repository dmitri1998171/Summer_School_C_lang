#include <ncurses.h>
#include <string.h>


void print_title(WINDOW *win, int starty, int startx, int width, char string[], chtype color){	
    int length, x, y;
	float temp;

	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();    
}

void winInit(WINDOW **win, int h, int w, int y, int x, char *str){
    *win = newwin(h, w, y, x);
    box(*win, 0, 0);
    wattron(*win, COLOR_PAIR(1));
    mvwprintw(*win, 0, 1, str);
    wattroff(*win, COLOR_PAIR(1));
    wrefresh(*win);
}
