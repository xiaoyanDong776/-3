#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024  // 定义最大行长度
#define TRUE 1        // 定义TRUE为1，用于while循环

int main(int argc, char **argv) {
    int sockfd, fd, n, m;
    char line[MAXLINE + 1];
    struct sockaddr_in6 servaddr, cliaddr;
    
    // 获取并打印当前时间
    time_t t0 = time(NULL); 
    printf("time #: %ld\n", t0); 
    fputs(ctime(&t0), stdout);

    // 创建一个IPv6 TCP套接字
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) 
        perror("socket error");

    // 初始化服务端地址结构并设置端口和地址
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin6_family = AF_INET6; 
    servaddr.sin6_port = htons(20000); 
    servaddr.sin6_addr = in6addr_any;

    // 将套接字绑定到服务端地址
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
        perror("bind error");

    // 开始监听端口上的入站连接
    if (listen(sockfd, 5) == -1) 
        perror("listen error");

    // 主循环，不断地接受新的连接
    while (TRUE) {
        printf("> Waiting clients ...\r\n"); 
        socklen_t clilen = sizeof(struct sockaddr);

        // 接受一个新的连接
        fd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (fd == -1) {
            perror("accept error");
        }
        printf("> Accepted.\r\n");

        // 读取并输出客户端发送的数据
        while ((n = read(fd, line, MAXLINE)) > 0) {
            line[n] = 0; // 确保字符串以null结束
            if (fputs(line, stdout) == EOF) 
                perror("fputs error");
        }

        // 关闭连接的套接字
        close(fd);
    }

    // 如果从read返回负值，则打印错误信息
    if (n < 0) 
        perror("read error");

    // 正常退出
    exit(0);
}