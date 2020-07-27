#include <menu.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *choices[] = {
                        "Choice 1",
                        "Choice 2",
                        "Choice 3",
                        "Choice 4",
                        "Exit",
                        (char *)NULL,
                  };
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{	
    ITEM **my_items;
	int c;				
	MENU *my_menu;
        WINDOW *win_left;
        WINDOW *win_right;
        int n_choices, i;
	
	/* Initialize curses */
	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);

	/* Create items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], choices[i]);

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the windows to be associated with the menu */
        win_left = newwin(LINES, COLS/2, 0, 0);
        keypad(win_left, TRUE);
     
        // my_menu_win = newwin(LINES, COLS/2, 0, COLS/2);
        // keypad(my_menu_win, TRUE);

	/* Set main window and sub window */
        set_menu_win(my_menu, win_left);
        set_menu_sub(my_menu, derwin(win_left, 6, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(win_left, 0, 0);

	print_in_middle(win_left, 1, 0, 40, "File Manager", COLOR_PAIR(1));
	mvwaddch(win_left, 2, 0, ACS_LTEE);
	mvwhline(win_left, 2, 1, ACS_HLINE, (COLS/2)-2);
	mvwaddch(win_left, 2, (COLS/2)-1, ACS_RTEE);
	mvprintw(LINES - 2, 0, "F1 to exit");
	refresh();
        
	/* Post the menu */
	post_menu(my_menu);
	wrefresh(win_left);

	while((c = wgetch(win_left)) != KEY_F(1))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
		}
                wrefresh(win_left);
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
	endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
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