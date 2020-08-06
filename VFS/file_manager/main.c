#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

char *choices_left[255];   // список всех файлов в директории
char *choices_right[255];
char *dir_arr[255];        // список папок в директории
char new_path[255];
char *path_left[255]={"/"}; 
char *path_right[255]={"/home/dmitry"};
int dir_size=0;
int size_left, size_right; 
int c, cycle = 1, win_tab = 0;
struct dirent **namelist_left;
struct dirent **namelist_right;
struct dirent *dir;
WINDOW *win_left;
WINDOW *win_right;
ITEM **my_items_left;
ITEM **my_items_right;
MENU *my_menu_left;
MENU *my_menu_right;
DIR *d;


void print_title(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void box_title(WINDOW* , int, int, int ,int, int, int, int);
void func(char *name);
void switchFunc(int*, MENU*);

void scaner(char path[], char *choices[], int *size){
	int i=0, f=0;
	
	d = opendir(path);
	if( d == NULL ){ perror("opendir"); exit(1); }

	while((dir = readdir(d)))
    {
		if(strcmp( dir->d_name, "." ) == 0 || 
			strcmp( dir->d_name, ".." ) == 0)
            { continue; }

        choices[i] = dir->d_name;

		if(dir->d_type == DT_DIR)
        {
			dir_arr[f] = dir->d_name;
			f++;
		} 
        printf("%s\n", choices[i]);
        i++;
	}
    *size = i;
	closedir(d);
}

void dirScan(char *path[], struct dirent **namelist, 
            char *choices[], int *size){

    *size = scandir(*path, &namelist, NULL, alphasort);
    if (*size < 0) perror("scandir");

    for(int i=0; i < *size; i++){    // заполняем массив для меню
        // if(strcmp( namelist[i]->d_name, "." ) == 0 || 
		// 	strcmp( namelist[i]->d_name, ".." ) == 0)
        //     { continue; }
        choices[i] = namelist[i]->d_name;
        
        if(namelist[i]->d_type == DT_DIR){
            dir_arr[dir_size] = namelist[i]->d_name;
            dir_size++;
        }
        free(namelist[i]);
    }
    free(namelist);   
}

void interfaceFunc(MENU **my_menu, WINDOW **win, ITEM ***my_item, char *path, int *size, int x){

    /* Создаем окно */
    *win = newwin(LINES-4, COLS/2, 3, x);
    keypad(*win, TRUE);

    /* Создаем меню */
    *my_menu = new_menu((ITEM **)*my_item);
    keypad(*win, TRUE);

    /* Задаем окно */
    set_menu_win(*my_menu, *win);                // отнош. меню к окну
    set_menu_sub(*my_menu, derwin(*win, *size, COLS/2-2, 3, 1)); // создание подокна
    set_menu_format(*my_menu, *size, 1);            // кол-во выводимых строк, столбцов за раз
    set_menu_mark(*my_menu, " * ");              // указатель текущ. эл-та

    print_title(*win, 1, 0, COLS/2, path, COLOR_PAIR(1));
    box_title(*win, 0, 0, 2, 1, COLS/2-2, 0, COLS/2-1);

    /* Публикуем меню */
	post_menu(*my_menu);
	wrefresh(*win);
}

void setItemsFunc(ITEM ***my_item, int *size, char *choices[]){
    *my_item = (ITEM **)calloc(*size, sizeof(ITEM *));
    for(int i = 0; i < *size; ++i){
        *my_item[i] = new_item(choices[i], 0);
        set_item_userptr(*my_item[i], func);
    }
}

int main(){	
    // получем список файлов в директории 
    // snprintf(new_path, sizeof new_path, "%s%s\0", path_right, dir_arr[1]);
    dirScan(path_left, namelist_left, choices_left, &size_left); 
    dirScan(path_right, namelist_right, choices_right, &size_right); 

    // scaner(path_left, choices_left, &size_left);  
    printf("\n size_left: %d\n size_right: %d\n", size_left, size_right);
    
    for(int i=0; i<size_right; i++)
    {
        printf("%s\n", choices_right[i]);
    }
    // setItemsFunc(&my_items_left, &size_left, choices_left);
    // setItemsFunc(&my_items_right, &size_right, choices_right);

    my_items_left = (ITEM **)calloc(size_left, sizeof(ITEM *));
    for(int i = 0; i < size_left; ++i){
        my_items_left[i] = new_item(choices_left[i], 0);
        set_item_userptr(my_items_left[i], func);
    }

    my_items_right = (ITEM **)calloc(size_right, sizeof(ITEM *));
    for(int i = 0; i < size_right; ++i){
        my_items_right[i] = new_item(choices_right[i], 0);
        set_item_userptr(my_items_right[i], func);
    }
// =========================================================
    
    // exit(0);

	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);

    interfaceFunc(&my_menu_left, &win_left, &my_items_left, *path_left, &size_left, 0);
    interfaceFunc(&my_menu_right, &win_right, &my_items_right, *path_right, &size_right, COLS/2);

    char *title="---------- File Manager ----------";
    print_title(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
	mvprintw(LINES - 1, 1, "Tab - switch panel  pageUp - scroll up  pageDown - scroll down  F1 - exit");
	refresh();

	while(cycle){
        /* переключение между окнами */
        if(win_tab == 0) {c = wgetch(win_left); switchFunc(&c, my_menu_left);}
        if(win_tab == 1) {c = wgetch(win_right); switchFunc(&c, my_menu_right);}        

        if(c == KEY_F(1)) cycle = 0;

        if(c == '\t'){ 
            win_tab += 1;
            if(win_tab > 1) win_tab = 0;
        }

        wrefresh(win_left);
        wrefresh(win_right);
	}	

	/* Удаляем меню и освождаем память */
    unpost_menu(my_menu_left);
    unpost_menu(my_menu_right);
    free_menu(my_menu_left);
    free_menu(my_menu_right);

    for(int i = 0; i < size_left; ++i){
        free_item(my_items_left[i]);
    }

    for(int i = 0; i < size_right; ++i){
        free_item(my_items_right[i]);
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
        
        case '\n':        // Enter 
        {	
            ITEM *cur;
            void (*p)(char *);

            cur = current_item(menu);
            p = item_userptr(cur);
            p((char *)item_name(cur));
            pos_menu_cursor(menu);
            break;
        }
        break;
    }
}

void func(char *name){
    // if(win_tab == 0)
    // {
    //     for(int i=0; i<dir_size; i++){
    //         if(strcmp(dir_arr[i], name) == 0){                
    //             // snprintf(&new_path, sizeof new_path, "%s%s\0", path_left, dir_arr[c-1]);
    //             // scaner(new_path);
    //             print_title(win_left, 1, 0, COLS/2, new_path, COLOR_PAIR(1));
    //             break;
    //         }
    //     }
    //     dirScan(new_path, namelist_left, choices_left, &size_left); 

    //     my_items_left = (ITEM **)calloc(size_left, sizeof(ITEM *));
    //     for(int i = 0; i < size_left; ++i){
    //         my_items_left[i] = new_item(choices_left[i], 0);
    //         // set_item_userptr(my_items_left[i], func);
    //     }
    //     my_menu_left = new_menu((ITEM **)my_items_left);

    //     set_menu_win(my_menu_left, win_left);           // отнош. меню к окну
    //     set_menu_sub(my_menu_left, derwin(win_left, LINES-7, (COLS/2)-2, 3, 1)); // создание подокна
    //     set_menu_format(my_menu_left, LINES-7, 1);      // кол-во выводимых строк, столбцов за раз


    //     wrefresh(win_left);
    // }
    // if(win_tab == 1)
    // {
    //     for(int i=0; i<dir_size; i++){
    //         if(strcmp(dir_arr[i], name) == 0){
    //             snprintf(new_path, sizeof new_path, "%s%s", path_right, name);
    //             print_title(win_right, 1, 0, COLS/2, new_path, COLOR_PAIR(1));
    //             break;
    //         }
    //     }
    //     dirScan(path_right, namelist_right, choices_right, &size_right); 

    //     my_items_right = (ITEM **)calloc(size_right, sizeof(ITEM *));
    //     for(int i = 0; i < size_right; ++i){
    //         my_items_right[i] = new_item(choices_right[i], 0);
    //         set_item_userptr(my_items_right[i], func);
    // }
    // }
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

/*
    TODO

\/ 1. Вывод во 2-ое окно
\/ 2. переключение окон
\/ 3. кол-во видимых эл-ов в меню 
4. переход по папкам
5. оптимизация кода
6. запуск приложений

*/