#include <ncurses.h>
#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <errno.h>

char *dir_arr[255];
char new_path[255];
char path_left[255]={"/"};
char *choices_left[255];
int size_left;
int highlight = 1;
struct dirent *dir;
DIR *d;
WINDOW *win_left;

void print_menu(WINDOW *menu_win, int highlight, char *choices[], int *size);
void switchFunc(int *c);
void enterFunc();

int scaner(char path[], char *choices[], int *size){
	int i=0, f=0;
	
	d = opendir(path);
	if( d == NULL ){ perror("opendir"); return 1; }

	while((dir = readdir(d))){
		if(strcmp( dir->d_name, "." ) == 0){
			continue;
	    }
        choices[i] = dir->d_name;

		if(dir->d_type == DT_DIR){
			dir_arr[f] = dir->d_name;
			f++;
		} 
        i++;
	}
    *size = i;
	closedir(d);
}

void box_title(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x){
    box(wnd, box_y, box_x);
	mvwaddch(wnd, line_y, lt_x, ACS_LTEE);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);
	mvwaddch(wnd, line_y, rt_x, ACS_RTEE);
}

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

void interfaceFunc(WINDOW **win, char *choices[], char *path, int *size, int x){

    /* Создаем окно */
    *win = newwin(LINES-4, COLS/2, 3, x);
    keypad(*win, TRUE);

    /* Рисуем границы */
    print_title(*win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    box_title(*win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

	wrefresh(*win);
}

int main()
{	
	int c, cycle = 1, win_tab = 0;
    

    scaner(path_left, choices_left, &size_left);
    printf("size: %d\n", size_left);
    
    // for(int i=0; i<size_left; i++){
    //     printf("%s\n", choices_left[i]);
    // }

	initscr();
    start_color();
	noecho();
	cbreak();	
    init_pair(1, COLOR_RED, COLOR_BLACK);

    char *title="---------- File Manager ----------";
    print_title(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - switch panel  F1 - exit");

    interfaceFunc(&win_left, choices_left, path_left, &size_left, 0);
    
    refresh();
	print_menu(win_left, highlight, choices_left, &size_left);

	while(cycle)
	{	
        /* переключение между окнами */
        if(win_tab == 0) {c = wgetch(win_left); switchFunc(&c);}
        // if(win_tab == 1) {c = wgetch(win_right); switchFunc(&c);}        

        if(c == KEY_F(1)) cycle = 0;

        if(c == '\t'){ 
            win_tab += 1;
            if(win_tab > 1) win_tab = 0;
        }

		print_menu(win_left, highlight, choices_left, &size_left);
	}	

	endwin();
	return 0;
}

void switchFunc(int *c){
    switch(*c){
        case KEY_DOWN:
            if(highlight == size_left)
                highlight = 1;
            else 
                ++highlight;
            break;
        case KEY_UP:
            if(highlight == 1)
                highlight = size_left;
            else
                --highlight;
            break;

        case 10:        // Enter 
            enterFunc();
            break;
    }
}

void enterFunc(){
	snprintf(new_path, sizeof new_path, "%s%s\0", path_left, choices_left[highlight-1]);
	wclear(win_left);
    scaner(new_path, choices_left, &size_left);

    print_title(win_left, 1, 0, COLS/2, new_path, COLOR_PAIR(1));
    box_title(win_left, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
}

void print_menu(WINDOW *menu_win, int highlight, char *choices[], int *size)
{
	int x, y, i;	

	x = 2;
	y = 3;
	box(menu_win, 0, 0);
	for(i = 0; i < *size; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}