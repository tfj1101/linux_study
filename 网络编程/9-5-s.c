#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#define SERVPORT 4444
#define MAXDATASIZE 256
#define STDIN 0
#define BACKLOG 10
int main(void)
{
    FILE *fp;

    int sockfd;
    int client_fd;

    int sin_size;
    int recvbytes;
    char buf[MAXDATASIZE];      // 用于聊天的缓冲区
    char buff[MAXDATASIZE];     // 用于输入用户名的缓冲区
    char send_str[MAXDATASIZE]; // 最多发出的字符不超过256个

    // 定义被select()监视的读、写、异常处理的文件描述符集合
    fd_set rfd_set; // 读
    fd_set wfd_set; // 写
    fd_set efd_set; // 异常

    // 本机地址信息、客户端地址信息
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;

    struct timeval timeout;

    int ret;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&my_addr, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    //inet_pton(AF_INET,"192.168.200.130",&my_addr.sin_addr);

    bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));

    listen(sockfd, BACKLOG);

    sin_size = sizeof(struct sockaddr_in);

    client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size);
    // 服务器设为非阻塞
    fcntl(client_fd, F_SETFD, O_NONBLOCK);

    recvbytes = recv(client_fd, buff, MAXDATASIZE, 0);

    buff[recvbytes] = '\0';

    fprintf(fp, "%s\n", buff);

    while (1)
    {
        FD_ZERO(&rfd_set);
        FD_ZERO(&wfd_set);
        FD_ZERO(&efd_set);
        FD_SET(STDIN, &rfd_set);
        FD_SET(client_fd, &rfd_set);
        FD_SET(client_fd, &wfd_set);
        FD_SET(client_fd, &efd_set);

        timeout.tv_sec = 10; // 被监视窗口待的秒数
        timeout.tv_usec = 0; // 微秒数

        ret = select(client_fd + 1, &rfd_set, &wfd_set, &efd_set, &timeout);
        if (ret == 0)
        {
            continue;
        }

        if (FD_ISSET(STDIN, &rfd_set))
        {
            fgets(send_str, MAXDATASIZE, stdin);
            send_str[strlen(send_str) - 1] = '\0';
            if (strncmp("quit", send_str, 4) == 0)
            {
                close(client_fd);
                close(sockfd);
                exit(0);
            }
            send(client_fd, send_str, strlen(send_str), 0);
        }

        if (FD_ISSET(client_fd, &rfd_set))
        {
            recvbytes = recv(client_fd, buf, MAXDATASIZE, 0);

            if (recvbytes == 0)
            {
                close(client_fd);
                close(sockfd);
                exit(0);
            }
            buf[recvbytes] = '\0';
            printf("%s:%s\n", buff, buf);
            printf("Server:");
            fflush(stdout);
        }

        if (FD_ISSET(client_fd, &efd_set))
        {
            close(client_fd);
            close(sockfd);
            exit(0);
        }
    }
}