#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 9527

void sys_error(char* str){  
    perror(str);
    exit(1);
}
int main(int argc, char* args[])
{
    int ret;
    int cfd;
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,SERV_IP, &serv_addr.sin_addr.s_addr);
    
    if(cfd < 0)
        sys_error("socket error");
    ret = connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret < 0){
        sys_error("connet error");
    }

    char buf[BUFSIZ];
    while(1){
        scanf("%s", buf);
        write(cfd, buf, strlen(buf));
        read(cfd, buf, strlen(buf));
        printf("%s\n", buf);
    }
    close(cfd);
    return 0;
}