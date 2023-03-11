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
#include<sys/select.h>
#include<iostream>
#include<algorithm>
#include"wrap.h"

#define SERV_PORT 8000

int main(int argc, char* args[])
{
    int lfd, maxfd, cfd;
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    //注意此处的maxfd必须初始化为lfd;
    maxfd = lfd;
    
    sockaddr_in serv_addr, clit_addr;
    socklen_t clit_addr_len = sizeof(clit_addr);


    memset(&clit_addr, 0, sizeof(clit_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd, (sockaddr*)& serv_addr, sizeof(serv_addr));
    /*
    请记住设置最大监听缓冲队列的长度,如果没有设置,select监听lfd时,lfd异常情况需要报告,rset中就会有lfd
    也就是说没有连接请求,lfd也会有反应,此时对lfd进行accpte会因为lfd未设置完全而出现参数错误
    */
    Listen(lfd, 128);
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    fd_set aset, rset;
    FD_ZERO(&rset);
    FD_ZERO(&aset);
    FD_SET(lfd, &aset);
    
    int client[FD_SETSIZE], nready = 0, client_len = 0;
    memset(client, -1, sizeof(client));
    char buf[BUFSIZ];

    while(1){
        rset = aset;
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        printf("nready = %d\n", nready);
        printf("FD_ISSET(lfd, &rset) :%s\n",FD_ISSET(lfd, &rset) ? "true" : "false");
        if(nready == -1){
            perror("select error");
            exit(1);
        }
        if(nready == 0){
            continue;
        }
        else{
            if(FD_ISSET(lfd, &rset)){
                cfd = Accept(lfd, (struct sockaddr*)&clit_addr, &clit_addr_len);
                client[client_len ++] = cfd;
                FD_SET(cfd, &aset);
                maxfd = std :: max(maxfd, cfd);
                if(-- nready == 0)
                    continue;
            }
            for(int i = 0; i < client_len; i ++){
                cfd = client[i];

                if(cfd == -1)
                    continue;

                if(FD_ISSET(cfd, &rset)){
                    int len = Read(cfd, buf, sizeof(buf));

                    if(len == 0){
                        client[i] = -1;
                        FD_CLR(cfd, &aset);
                        if(maxfd == cfd){
                            maxfd = -1;
                            for(int j = 0; j < client_len; j ++){
                                    maxfd = std :: max(maxfd, client[j]);
                            }
                        }
                        close(cfd);
                    }

                    else{
                        for(int j = 0; j < len; j++){
                            buf[j] = toupper(buf[j]);
                        }
                        Write(cfd, buf, len);
                    }
                    if(-- nready == 0)
                        break;
                }
           } 
            
        }
    }

}    
    

    

    
    
