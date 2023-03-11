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