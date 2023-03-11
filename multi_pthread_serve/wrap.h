#ifndef _WRAP_
#define _WRAP_
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>

int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int Listen(int sockfd, int backlog);

#endif

