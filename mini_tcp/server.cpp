#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"wrap.h"

#define SERV_PORT 9527
void sys_error(char const *str){
    perror(str);
    exit(1);
}

int main(int argc, char* args[])
{
    int ret;
    int lfd,afd;
    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    if(lfd < 0)
        sys_error("Socket for listen"); 
    
    struct sockaddr_in serv_addr, clit_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = Bind(lfd, (sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret < 0){
        sys_error("Bind error");
    }

    ret = listen(lfd, 128);
    if(ret < 0){
        sys_error("listen error");
    }

    socklen_t clit_addr_len = sizeof(clit_addr);
    afd = accept(lfd, (struct sockaddr*)&clit_addr, &clit_addr_len);

    char clit_ip[BUFSIZ];
    socklen_t clit_ip_len = sizeof(clit_ip);
    printf("clit_addr is %s, port is %d\n", 
    inet_ntop(AF_INET, (void*)&clit_addr.sin_addr.s_addr, clit_ip, clit_ip_len),
    ntohs(clit_addr.sin_port)
    );

    char buf[BUFSIZ];
    while(1){
        int len = read(afd, buf, sizeof(buf));
        if(len < 0){
            sys_error("read error");
        }
        //printf("%s\n", buf);
        
        for(int i = 0; i < len; i ++)
            buf[i] = toupper(buf[i]);
        
        ret = write(afd, buf, len);
        if(ret < 0){
            sys_error("wrte error");
        }    
    }   
    close(lfd);
    close(afd);

    return 0;
}