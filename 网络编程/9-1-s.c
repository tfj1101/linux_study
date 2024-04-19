#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SERVPORT 8888
#define BACKLOG 10
int main(void)
{
    int socketfd, clientfd;
    struct sockaddr_in myaddr;
    struct sockaddr_in remote;
    int sin_size;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket 创建失败！");
        exit(1);
    }
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(SERVPORT);
    memset(&(myaddr.sin_zero), 0, 8);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind 失败！\n");
        exit(1);
    }
    if (listen(socketfd, BACKLOG) == -1)
    {
        perror("listen 失败！\n");
        exit(1);
    }
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((clientfd = accept(socketfd, (struct sockaddr *)&remote, &sin_size)) == -1)
        {
            perror("accept 失败！\n");
            continue;
        }
        printf("收到一个连接来自：%s\n,", inet_ntoa(remote.sin_addr));
        if (fork() != 0)
        {
            if ((send(clientfd, "连接上了", 26, 0)) == -1)
            {
                perror("send 失败！\n");
                close(clientfd);
                exit(0);
            }
        }
        close(clientfd);
    }
}