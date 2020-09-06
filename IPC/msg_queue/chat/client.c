#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

#define MSGSZ 256

void print_title(WINDOW *win, int starty, int startx, int width, char string[], chtype color);
void winInit(WINDOW **win, int h, int w, int y, int x, char *str);

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

char str[15];
int msqid_s, msqid;
int strCount = 1;
pid_t pid;
pthread_t tid, tid2;
size_t buf_length;
message_buf sbuf;
WINDOW *inputWin, *outputWin;

void *writeFunc(void *param){
    // char *name = (char *) param;

    while(1){
        // заполняем структуру
        mvwgetstr(inputWin, 1, 1, str);  // получ. строки
        wclear(inputWin);
        winInit(&inputWin, 3, COLS, LINES-3, 0, "Enter: ");
        wmove(inputWin, 1, 1);

        snprintf(sbuf.mtext, sizeof(sbuf.mtext), 
                "%d: %s", pid, str);
        sbuf.mtype = 1;
        buf_length = strlen(sbuf.mtext) + 1;

        // отпр. сообщ.
        if (msgsnd(msqid, &sbuf, MSGSZ, IPC_NOWAIT) < 0){
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
        
        // вывод строки на экран

        // mvwprintw(outputWin, 2, 1, sbuf.mtext);
        // wrefresh(outputWin);

        mvwprintw(outputWin, strCount, 2, "%s\n", sbuf.mtext);
        wrefresh(outputWin);
        strCount++;
    }
    return NULL;
}

int main(int argc, char *argv[]){
    char *title = "---------- Chat ----------";
    int msgflg = IPC_CREAT | 0666;
    key_t key=10;

    pid = getpid();

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
      
    pthread_create(&tid, NULL, writeFunc, NULL);
    pthread_create(&tid2, NULL, readFunc, NULL);

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);

    endwin();
    return 0;
}