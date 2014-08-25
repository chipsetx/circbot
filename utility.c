#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "utility.h"
#include "circbot.h"

void sendserver(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  ret = vsnprintf(sendbuf, BUFFERSIZE-1, fmt, args);
  send(G_socketfd, sendbuf, strlen(sendbuf), 0);
  va_end(args);
}

void init_users() {
  FILE *fp = fopen("users", "r");
  char *line = malloc(BUFFERSIZE);

  num_users=0;
  while (fgets(line, BUFFERSIZE-1, fp)) {
    *(line+strlen(line)-1) = '\0'; // Drop the trailing \n
    users[num_users] = malloc(strlen(line)*sizeof(char)); // TODO: Proper user string length
    strcpy(users[num_users], line);
    num_users += 1;
  }

  G_owner = malloc(strlen(users[0])); // TODO: Proper owner string length
  sscanf(users[0], "%[^!]s", G_owner);

  free(line);
  fclose(fp);
}

int is_authorized(char *user) {
  int i;
  for(i=0; i<num_users; ++i) {
    if(strcmp(users[i], user) == 0) {
      return 1;
    }
  }
  return 0;
}
