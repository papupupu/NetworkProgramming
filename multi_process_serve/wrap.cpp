#include"wrap.h"

void error_exit(char const *str){
    perror(str);
    exit(1);
}

int Bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
    int n;
    n = bind(sockfd, addr, addrlen);
    if(n < 0){
       error_exit("bind error");
    }
    return n;
}

int Socket(int domain, int type, int protocol){
    int n;
    n = socket(domain, type, protocol);
    if(n < 0){
        error_exit("socket error");
    }
    return n;
}


int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int n;
    n = accept(sockfd, addr, addrlen);
    if(n == -1){
        perror("accept error");
        exit(1);
    }
    return n;
}

ssize_t Read(int fd, void *buf, size_t count)
{
    int n;
    n = read(fd, buf, count);
    if(n < 0){
        perror("read error");
        exit(1);
    }
    if(!n)
        exit(0);
    return n;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
   int n;
   n = write(fd, buf, count);
   if(n < 0){
        perror("write error");
        exit(1);
   } 
   return n;
}


int Sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    int n = sigaction(signum, act, oldact);
    if(n < 0){
        perror("sigaction error");
        exit(1);
    }
    return n;
}

int Listen(int sockfd, int backlog)
{
    int n = listen(sockfd, backlog);
    if(n == -1){
        perror("listen error");
        exit(1);
    }
    return n;
}