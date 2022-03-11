#define msize 5

#define MAXPENDING 5   
#define RCVBUFSIZE 32   

int A[msize][msize];
int vector[msize];
int str_m[msize*msize];
int sock;
struct sockaddr_in echoServAddr;
