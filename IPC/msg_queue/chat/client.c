#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
    unsigned int pid;
} message_buf;

char str[15];
int msqid_s, msqid;
int pid;
pthread_t tid, tid2;
size_t buf_length;
message_buf sbuf;
WINDOW *inputWin, *outputWin;

void *writeFunc(void*);
void *readFunc(void*);

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

int main(int argc, char *argv[]){
    char *title = "---------- Chat ----------";
    int msgflg = IPC_CREAT | 0666;
    key_t key=10;

    pid  = getpid();

    initscr();
    start_color(); 
    init_color(COLOR_BLACK, 140, 140, 140);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    print_title(stdscr, 1, 0, COLS, title, COLOR_PAIR(1));
    winInit(&inputWin, 3, COLS, LINES-3, 0, "Enter: ");
    winInit(&outputWin, LINES-6, COLS, 3, 0, "Out: ");

    // создаем очередь(отпр.)
    if ((msqid = msgget(key, msgflg)) < 0){
        perror("msgget"); exit(1); }

    // получ.
    if ((msqid_s = msgget(pid, msgflg)) < 0){
        perror("msgget"); exit(1); }
      
    pthread_create(&tid, NULL, writeFunc, argv[1]);
    pthread_create(&tid2, NULL, readFunc, NULL);

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);

    endwin();
    return 0;
}

void *writeFunc(void *param){
    char *name = (char *) param;

    while(1){
        // заполняем структуру
        mvwgetstr(inputWin, 1, 1, str);  // получ. строки
        wclear(inputWin);
        winInit(&inputWin, 3, COLS, LINES-3, 0, "Enter: ");
        wmove(inputWin,1,1);

        snprintf(sbuf.mtext, sizeof(sbuf.mtext), 
                "%s: %s", name, str);
        sbuf.mtype = 1;
        sbuf.pid = pid;
        size_t buf_length = strlen(sbuf.mtext) + 1;

        mvwprintw(inputWin, 0, 10, "pid: %d\n", sbuf.pid);

        // отпр. сообщ.
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0){
            printf ("%d, %ld, %s, %ld\n", 
                msqid, sbuf.mtype, sbuf.mtext, buf_length);
            perror("msgsnd"); exit(1); }
    }
    return NULL;
}

void *readFunc(void *param){
    while(1){
        // получ. ответ
        if (msgrcv(msqid_s, &sbuf, MSGSZ, 1, 0) < 0){
            perror("msgrcv"); exit(1);}
        
        mvwprintw(outputWin, 1, 1, sbuf.mtext); // вывод строки на экран
        wrefresh(outputWin);
    }
    return NULL;
}