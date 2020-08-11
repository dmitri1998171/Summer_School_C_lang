#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

int main()
{
    initscr();

    for (int n = 0; n < 10; n++)
    {
        mvaddch(0,n,'#');
        refresh();
        usleep(100000);
    }
    getch();
    endwin();
}