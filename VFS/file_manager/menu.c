#include <ncurses.h>
#include <dirent.h> 
#include <sys/types.h> 
#include <sys/param.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <errno.h>

#define ARR_SIZE 255

char *dir_arr_left[ARR_SIZE];
char *dir_arr_right[ARR_SIZE];
char new_path[ARR_SIZE];
char path_left[ARR_SIZE]={"/"};
char path_right[ARR_SIZE]={"/"};
char *choices_right[ARR_SIZE];
char *choices_left[ARR_SIZE];
int size_left, size_right;
int dir_size_left, dir_size_right;
int highlight_left = 1, highlight_right = 1;
struct dirent *dir;
DIR *d;
WINDOW *win_left;
WINDOW *win_right;

void print_menu(WINDOW *menu_win, int highlight, char *choices[], int *size);
void switchFunc(int *, int *, int *);
void enterFunc(WINDOW *, char **, char *, char **, int *, int *, int *);

void scaner(char path[], char *choices[], char *dir_arr[], int *size, int *dir_size){
	int i=0, f=0;
	
	d = opendir(path);
	if( d == NULL ){ perror("opendir"); exit(1); }

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
    *dir_size = f;
	closedir(d);
}

void box_title(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x){
    box(wnd, box_y, box_x);
	// mvwaddch(wnd, line_y, lt_x, ACS_LTEE);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);
	// mvwaddch(wnd, line_y, rt_x, ACS_RTEE);
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

void interfaceFunc(WINDOW **win, char *path, int *size, int x){


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
    
    // getcwd(path_left, ARR_SIZE);

    scaner(path_left, choices_left, dir_arr_left, &size_left, &dir_size_left);
    scaner(path_right, choices_right, dir_arr_right, &size_right, &dir_size_right);
    printf("\n");

	initscr();
    start_color();
	noecho();
	cbreak();	
    init_pair(1, COLOR_RED, COLOR_BLACK);

    char *title="---------- File Manager ----------";
    print_title(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - switch panel  F1 - exit");

    interfaceFunc(&win_left, path_left, &size_left, 0);
    interfaceFunc(&win_right, path_right, &size_right, COLS/2);
    refresh();

	print_menu(win_left, highlight_left, choices_left, &size_left);
    print_menu(win_right, highlight_right, choices_right, &size_right);

	while(cycle)
	{	
        /* переключение между окнами */
        if(win_tab == 0){
            c = wgetch(win_left); 
            switchFunc(&c, &highlight_left, &size_left);
            if(c == 10) enterFunc(win_left, choices_left, path_left, dir_arr_left, &highlight_left, &size_left, &dir_size_left);
            print_menu(win_left, highlight_left, choices_left, &size_left);
        }
        if(win_tab == 1){
            c = wgetch(win_right);
            switchFunc(&c, &highlight_right, &size_right);
            if(c == 10) enterFunc(win_right, choices_right, path_right, dir_arr_right, &highlight_right, &size_right, &dir_size_right);
            print_menu(win_right, highlight_right, choices_right, &size_right);   
        }

        if(c == KEY_F(1)) cycle = 0;

        if(c == '\t'){ 
            win_tab += 1;
            if(win_tab > 1) win_tab = 0;
        }
	}	

	endwin();
	return 0;
}

void switchFunc(int *c, int *highlight, int *size){
    switch(*c){
        case KEY_DOWN:
            if(highlight == size)
                *highlight = 1;
            else 
                ++*highlight;  
            break;
        case KEY_UP:
            if(*highlight == 1)
                highlight = size;
            else
                --*highlight;
            break;
    }
}

// void enterFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int highlight, int *size, int *dir_size){
//     // for(int i=0; i < *dir_size; i++){
//     //     if(strcmp(choices[highlight-1], dir_arr[i]) == 0){
//             snprintf(new_path, sizeof new_path, "%s%s", path, choices[highlight-1]);
            
//             wclear(win);
//             scaner(new_path, choices, dir_arr_left, size, dir_size);
            
//             print_title(win, 1, 0, COLS/2, new_path, COLOR_PAIR(1));
//             box_title(win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
//         // }
//         // else {mvwprintw(win, 1,3, "hello");}
//     // }
// }


void enterFunc(WINDOW *win, char *choices[], char *path, char *dir_arr[], int *highlight, int *size, int *dir_size){   
    // chdir(choices[*highlight-1]);
    // getcwd(new_path, ARR_SIZE);

    wclear(win);
    scaner(new_path, choices, dir_arr_left, size, dir_size);
    
    print_title(win, 1, 0, COLS/2, new_path, COLOR_PAIR(1));
    box_title(win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);
    *highlight = 1;
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