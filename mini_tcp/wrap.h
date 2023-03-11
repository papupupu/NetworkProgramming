#ifndef _WRAP_
#define _WRAP_
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <errno.h>

int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);



#endif

