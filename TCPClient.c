#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1024  // 定义最大行长度
#define TRUE 1        // 定义TRUE为1，用于循环条件

int main(int argc, char **argv) {
    int sockfd, n, m;
    char line[MAXLINE + 1]; 
    struct sockaddr_in6 servaddr; 

    // 获取并打印当前时间
    time_t t0 = time(NULL); 
    printf("time #: %ld\n", t0); 
    fputs(ctime(&t0), stdout);

    // 检查命令行参数数量
    if (argc != 2) {
        perror("usage: a.out <IPaddress>");
        exit(1);
    }

    // 创建IPv6 TCP套接字
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    // 初始化服务端地址结构
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin6_family = AF_INET6; 
    servaddr.sin6_port = htons(20000);

    // 将IP地址从文本转换为二进制形式
    if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    // 从标准输入读取数据，并发送到服务器
    while (fgets(line, MAXLINE, stdin) != NULL) {
        send(sockfd, line, strlen(line), 0);
    }

    // 关闭套接字
    close(sockfd);
    exit(0);
}