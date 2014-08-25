#define BUFFERSIZE 512*1024
char sendbuf[BUFFERSIZE];
char recvbuf[BUFFERSIZE];

int G_socketfd;
int G_logfd;

ssize_t ret;

void handler(int s);
void (*quit)(int);
