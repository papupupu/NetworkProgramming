#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<ctype.h>
#include<errno.h>

/*
    bind();
    listen();
    accept();
    while(true)

*/
#define SERV_PORT 9527
void sys_err(char const *str){
    perror(str);
    exit(1);
}
int main(int argc, char* args[])
{
    int ret;
    int lfd, afd;
    lfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(lfd < 0){
        sys_err("socket error");
    }
    
    struct sockaddr_in serv_addr, clit_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret < 0){
        sys_err("bind error");
    }
    
    ret = listen(lfd, 128);
    if(ret < 0){
        sys_err("listen error");
    }
    
    socklen_t clit_addr_len = sizeof(clit_addr);
    afd = accept(lfd, (struct sockaddr*)&clit_addr, &clit_addr_len);
    if(afd < 0){
        sys_err("accept error");
    }

    char buf[BUFSIZ];
    while(1){
       int len = read(afd, buf, BUFSIZ);
       if(len < 0){
            sys_err("read error");
       }
       for(int i = 0; i < len; i ++){
            buf[i] = toupper(buf[i]);
       }
       ret = write(afd, buf, len);
       if(ret < 0){
            sys_err("write");
       } 
    }
    close(lfd);
    close(afd);
}