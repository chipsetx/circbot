#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>

#include "circbot.h"
#include "utility.h"

int main(int argc, char **argv) {

    if (argc < 5) {
        printf("Usage: %s <alias> <server> <port> <channel>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *alias = argv[1];
    char *server = argv[2];
    char *port = argv[3];
    char *channel = argv[4];

    signal(SIGINT, handler);

    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    ret = getaddrinfo(server, port, &hints, &servinfo);
    if (ret) perror("getaddrinfo");

    G_socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (G_socketfd == -1) perror("socket");

    ret = connect(G_socketfd,servinfo->ai_addr, servinfo->ai_addrlen);
    if (ret == -1) {
        close(G_socketfd);
        perror("connet");
    }

    init_users();

    sendserver("NICK %s\r\n", alias);
    sendserver("USER %s 8 * :%s\r\n", alias, alias);
    sendserver("JOIN #%s\r\n", channel);

    G_logfd = open("output", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR);
    // TODO: Receive MOTD first

    sendserver("PRIVMSG #%s :Hello from C!\r\n", channel);

    // TODO: Parse received messages properly
    char *pos;
    while(1) {
        ret = recv(G_socketfd, recvbuf, BUFFERSIZE-1, 0);
        recvbuf[ret] = '\0';
        write(G_logfd, recvbuf, ret);

        pos = strstr(recvbuf, "PING");
        if (pos) {
            char *server = pos + 5;
            sendserver("PONG %s\r\n", server);
        }

        pos = strstr(recvbuf, ".op");
        if (pos) {
          char *sender = malloc(BUFFERSIZE);
          sscanf(recvbuf, ":%s", sender);
          if (is_authorized(sender)) {
            if (strlen(pos) > 5) { // If more than '.op\r\n'
              char *user = pos + 4;
              sendserver("MODE #%s +o %s\r\n", channel, user);
            } else {
              char *user = malloc(BUFFERSIZE);
              sscanf(recvbuf, ":%[^!]s", user);
              sendserver("MODE #%s +o %s\r\n", channel, user);
            }
          }
          else {
            sendserver("PRIVMSG #%s :You are not authorized for .op.\r\n", channel);
          }

        }

        pos = strstr(recvbuf, ".quit");
        if (pos) {
          quit(SIGINT); // Calls same routine as a SIGINT, so fake it.
        }

        pos = strstr(recvbuf, ".topic");
        if (pos) {
          // TODO: Add authentication
          sendserver("TOPIC #%s :%s\r\n", channel, pos+7);
        }

        pos = strstr(recvbuf, ".time");
        if (pos) {
          time_t cur_time = time(NULL);

          sendserver("PRIVMSG #%s :Vancouver: %s\r\n", channel, asctime(localtime(&cur_time)));
          usleep(250*1000);

          cur_time += 3*3600;
          sendserver("PRIVMSG #%s :New York: %s\r\n", channel, asctime(localtime(&cur_time)));
          usleep(250*1000);

          cur_time += 5*3600;
          sendserver("PRIVMSG #%s :London: %s\r\n", channel, asctime(localtime(&cur_time)));
          usleep(250*1000);
        }
    }

    return 0;
}

void handler(int s) {
    sendserver("QUIT :Goodbye from C\r\n");
    // Trigger a FIN
    shutdown(G_socketfd, SHUT_WR);
    while (ret = recv(G_socketfd, recvbuf, BUFFERSIZE-1, 0)) {
        write(G_logfd, recvbuf, ret);
    }
    close(G_socketfd);
    close(G_logfd);
    exit(EXIT_SUCCESS);
}
void (*quit)(int) = &handler;
