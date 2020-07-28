#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

char *choices[];

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{	
    struct dirent **namelist;
    int n, c, n_choices;
    ITEM **my_items;
	MENU *my_menu;
    WINDOW *win_left;
    WINDOW *win_right;

//======== получем список файлов в директории =============

    n = scandir(".", &namelist, NULL, alphasort);
    if (n < 0) perror("scandir");
    
    for(int i=0; i<n; i++){    // заполняем массив для меню
        choices[i] = namelist[i]->d_name;
        free(namelist[i]);
    }
    free(namelist);

//=========================================================
	
    /* Создаем эл-ты */
    n_choices = sizeof(&choices);
    my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i){
        my_items[i] = new_item(choices[i], 0);
    }

	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);


	/* создаем меню */
	my_menu = new_menu((ITEM **)my_items);

    win_left = newwin(LINES-1, COLS/2, 0, 0);
    keypad(win_left, TRUE);
    
    win_right = newwin(LINES-1, COLS/2, 0, COLS/2);
    keypad(win_right, TRUE);

	/* Set main window and sub window */
        set_menu_win(my_menu, win_left);
        set_menu_sub(my_menu, derwin(win_left, 6, 38, 3, 1));
        set_menu_format(my_menu, 5, 1);
        // set_menu_win(my_menu, win_right);
        // set_menu_sub(my_menu, derwin(win_right, 6, 38, 3, 1));

    set_menu_mark(my_menu, " * ");

	/* Рисуем границы и заголовок */
	print_in_middle(win_left, 1, 0, COLS/2, "File Manager", COLOR_PAIR(1));
    print_in_middle(win_right, 1, 0, COLS/2, "File Manager", COLOR_PAIR(1));

    box(win_left, 0, 0);
	mvwaddch(win_left, 2, 0, ACS_LTEE);
	mvwhline(win_left, 2, 1, ACS_HLINE, (COLS/2)-2);
	mvwaddch(win_left, 2, (COLS/2)-1, ACS_RTEE);
        
    box(win_right, 0, 0);
    mvwaddch(win_right, 2, (COLS/2)-1, ACS_RTEE);
	mvwhline(win_right, 2, 1, ACS_HLINE, (COLS/2)-2);
	mvwaddch(win_right, 2, 0, ACS_LTEE);
    
	/* Публикуем меню */
	post_menu(my_menu);
	wrefresh(win_left);
    wrefresh(win_right);

	mvprintw(LINES - 1, 1, "Tab - switch panel  pageUp - scroll up  pageDown - scroll down  F1 - exit");
	refresh();

	while((c = wgetch(win_left)) != KEY_F(1))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
            case KEY_NPAGE:
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(my_menu, REQ_SCR_UPAGE);
				break;
		}
                wrefresh(win_left);
	}	

	/* Удаляем меню и освождаем память */
    unpost_menu(my_menu);
    free_menu(my_menu);
    for(int i = 0; i < n_choices; ++i){
            free_item(my_items[i]);
    }
	endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
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