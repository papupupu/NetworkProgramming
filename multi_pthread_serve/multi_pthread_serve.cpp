#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<iostream>
#include"wrap.h"

#define SERV_PORT 8000
class sock_info{
public:
    sock_info(sockaddr_in addr, int fd):m_sock_addr(addr), m_fd(fd) {}
    struct sockaddr_in m_sock_addr;
    int m_fd;
};

void* myfunc(void* sock_info_p){
    char buf[BUFSIZ];
    sock_info* info_p = (sock_info*) sock_info_p;
    std :: cout << "this is pthrad : " << pthread_self() << std :: endl;
    while(1){
        int len = Read(info_p -> m_fd, buf, sizeof(buf));
        if(len == 0) {
            std :: cout << "clit end" << std :: endl;
            break;
        }
        for(int i= 0; i < len; i ++){
            buf[i] = toupper(buf[i]);
        }
        Write(info_p -> m_fd, buf, len);
    }
    close(info_p -> m_fd);
    delete  info_p;
    pthread_exit(NULL);
}

int main(int argc, char* args[])
{
    int lfd, afd;
    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt  = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));


    struct sockaddr_in serv_addr, clit_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    

    Bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    Listen(lfd, 128);

    sock_info* sock_info_p;
    socklen_t clit_addr_len = sizeof(clit_addr);

    while(1){
        afd = Accept(lfd, (struct sockaddr*)& clit_addr, &clit_addr_len); 
        sock_info_p = new sock_info(clit_addr, afd); 
        pthread_t tid;
        if(pthread_create(&tid, NULL, myfunc, (void*)sock_info_p) != 0){
            perror("pthread_create error");
            pthread_exit((void*)1);
        }
        
        if(pthread_detach(tid) != 0){
            perror("pthread_detach error");
            pthread_exit((void*)1);
        } 
    }
    return 0;
}
