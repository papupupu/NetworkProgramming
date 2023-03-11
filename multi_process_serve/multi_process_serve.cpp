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
#include"wrap.h"

#define SERV_PORT 8000


void catch_child(int signum){
    //如果子进程状态变化,进行回收尝试,成功则继续回收,无僵尸子进程回收则waitpid返回0,函数结束;
    while(waitpid(0, NULL, WNOHANG) > 0);
}

int main(int argc, char* args[])
{
    int lfd, cfd;
    //设置套接字;
    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    socklen_t clit_addr_len;
    struct sockaddr_in serv_addr, clit_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将套接字和serv_ip + port绑定;
    Bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //设置能同时存放已经完成了三次握手但是还未经过accept函数处理的的连接的最大个数;
    Listen(lfd, 20);

    //printf("error in this\n");

    pid_t pid;
    while(1){
        //阻塞从监听队列中取出连接并返回新的套接字用于客户端进行通信,
        //注意必须在while中调用accept,因为它会阻塞,建立一个连接后再fork,这样不会一直fork下去;
        //prinf("error in this\n");
        clit_addr_len = sizeof(clit_addr);
        //printf("&clit_addr_len = %lld\n", &clit_addr_len);
        cfd = Accept(lfd, (struct sockaddr*)&clit_addr, &clit_addr_len);

        //fork子进程
        pid = fork();

        //子进程退出循环
        if(!pid)
            break;

        //父进程关闭cfd;
        close(cfd);

        //父进程设置捕捉函数用于回收子进程;
        struct sigaction act;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_handler = catch_child;
        Sigaction(SIGCHLD, &act, NULL);
    }

    //子进程处理客户端请求;
    if(!pid){
        close(lfd);
        char buf[BUFSIZ];
        while(1){
            int len = Read(cfd, buf, sizeof(buf));
            for(int i = 0; i < len; i ++)
                buf[i] = toupper(buf[i]);
            Write(cfd, buf, len);
        }
        close(cfd);
        exit(0);
   }

   //父进程结束
   if(pid){
    close(lfd);
    exit(0);
   }
}
