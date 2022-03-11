#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include "Variables.h"

void generate(void);
void outFunc(char);
void DieWithError(char*);
void ServerSendFunc(int);
void ServerRecvFunc(int);
int CreateTCPServerSocket(unsigned short);
int AcceptTCPConnection(int);