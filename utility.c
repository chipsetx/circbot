#include <stdio.h>
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
