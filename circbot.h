#define BUFFERSIZE 512*1024
char sendbuf[BUFFERSIZE];
char recvbuf[BUFFERSIZE];

int G_socketfd;
int G_logfd;

ssize_t ret;

char *G_owner;
int num_users;
char *users[32]; // TODO: Dynamically size users array

void handler(int s);
void (*quit)(int);
