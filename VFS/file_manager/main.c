#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

char *choices[];
char *dir_arr[];
char *dir_path;
int arr_size;
WINDOW *win_left;
WINDOW *win_right;

void print_title(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void box_title(WINDOW* , int, int, int ,int, int, int, int);
void func(char *name);
void switchFunc(int*, MENU*);

void resize_array(char *arr[], char *new_arr[]){
    new_arr = calloc(sizeof(&arr), 1);
}

int main(){	
    char *path=".";
    int size;
    int c, n_choices, cycle = 1, win_tab = 0;;
    struct dirent **namelist;
    ITEM **my_items_left;
    ITEM **my_items_right;
	MENU *my_menu_left;
    MENU *my_menu_right;

//======== получем список файлов в директории =============

    // strcat(path, "home/dmitry");
    // resize_array(path, path);

    size = scandir(path, &namelist, NULL, alphasort);
    if (size < 0) perror("scandir");

    // arr_size = size;
    // dir_path = path;

    printf("\nsize: %d\n\n", size);
    printf("all\tfolder\n");
    printf("------------------------------\n");

    int j=0;
    for(int i=0; i<size; i++){    // заполняем массив для меню
        choices[i] = namelist[i]->d_name;
        
        if(namelist[i]->d_type == DT_DIR){
            dir_arr[i] = namelist[i]->d_name;
        }
        
        printf("%s\t\t\t%s\n", choices[i], dir_arr[i]);
        free(namelist[i]);
    }
    free(namelist);

//======== Создаем эл-ты ==================================
	
    my_items_right = (ITEM **)calloc(size+1, sizeof(ITEM *));
    for(int i = 0; i < size; ++i){
        my_items_right[i] = new_item(choices[i], 0);
        set_item_userptr(my_items_right[i], func);
    }

    my_items_left = (ITEM **)calloc(size+1, sizeof(ITEM *));
    for(int i = 0; i < size; ++i){
        my_items_left[i] = new_item(choices[i], 0);
        set_item_userptr(my_items_left[i], func);
    }
    
//=========================================================

    // exit(0);

	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* создаем меню */
	my_menu_left = new_menu((ITEM **)my_items_left);
    my_menu_right = new_menu((ITEM **)my_items_right);

    win_left = newwin(LINES-1, COLS/2, 0, 0);
    keypad(win_left, TRUE);
    
    win_right = newwin(LINES-1, COLS/2, 0, COLS/2);
    keypad(win_right, TRUE);

	/* Задаем окно */
    set_menu_win(my_menu_left, win_left);
    set_menu_sub(my_menu_left, derwin(win_left, 6, (COLS/2)-2, 3, 1));
    set_menu_format(my_menu_left, 5, 1);

    set_menu_win(my_menu_right, win_right);
    set_menu_sub(my_menu_right, derwin(win_right, 6, (COLS/2)-2, 3, 1));
    set_menu_format(my_menu_right, 5, 1);

    set_menu_mark(my_menu_left, " * ");
    set_menu_mark(my_menu_right, " * ");

	/* Рисуем границы и заголовок */
	print_title(win_left, 1, 0, COLS/2, "File Manager", COLOR_PAIR(1));
    print_title(win_right, 1, 0, COLS/2, "File Manager", COLOR_PAIR(1));

    box_title(win_left, 0, 0, 2, 1, (COLS/2)-2, 0, (COLS/2)-1);
    box_title(win_right, 0, 0, 2, 1, (COLS/2)-2, 0, (COLS/2)-1);
    
	/* Публикуем меню */
	post_menu(my_menu_left);
    post_menu(my_menu_right);
	wrefresh(win_left);
    wrefresh(win_right);

	mvprintw(LINES - 1, 1, "Tab - switch panel  pageUp - scroll up  pageDown - scroll down  F1 - exit");
	refresh();

	while(cycle)
	{      
        if(win_tab == 0) {c = wgetch(win_left); switchFunc(&c, my_menu_left);}
        if(win_tab == 1) {c = wgetch(win_right); switchFunc(&c, my_menu_right);}        

        if(c == KEY_F(1)) cycle = 0;

        if(c == '\t'){ 
            win_tab += 1;
            if(win_tab > 1) win_tab = 0;
        }

            // case 10:        // Enter 
			// {	
            //     ITEM *cur;
			// 	void (*p)(char *);

			// 	cur = current_item(my_menu_left);
			// 	p = item_userptr(cur);
			// 	p((char *)item_name(cur));
			// 	pos_menu_cursor(my_menu_left);
			// 	break;
			// }
            //     break;
		
        wrefresh(win_left);
        wrefresh(win_right);
	}	

	/* Удаляем меню и освождаем память */
    unpost_menu(my_menu_left);
    unpost_menu(my_menu_right);
    free_menu(my_menu_left);
    free_menu(my_menu_right);
    for(int i = 0; i < n_choices; ++i){
        free_item(my_items_right[i]);
        free_item(my_items_left[i]);
    }
	endwin();
}

void switchFunc(int *c, MENU *menu){
    switch(*c){
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            menu_driver(menu, REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            menu_driver(menu, REQ_SCR_UPAGE);
            break;
    }
}

void func(char *name){	
    int check = 0;

    for(int i=0; i<arr_size; i++){
        if(strcmp(dir_arr[i], name) == 0){strcat(dir_path, name); break;}
    }

    mvwprintw(win_left, 7,3, dir_path);
}	

void box_title(WINDOW *wnd, int box_x, int box_y, int line_y, int line_x, int line_w, int lt_x, int rt_x){
    box(wnd, box_y, box_x);
	mvwaddch(wnd, line_y, lt_x, ACS_LTEE);
	mvwhline(wnd, line_y, line_x, ACS_HLINE, line_w);
	mvwaddch(wnd, line_y, rt_x, ACS_RTEE);
}

void print_title(WINDOW *win, int starty, int startx, int width, char *string, chtype color){	
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

/*
    TODO

\/ 1. Вывод во 2-ое окно
\/ 2. переключение окон
3. кол-во видимых эл-ов в меню 
4. переход по папкам
5. permission denied 

*/