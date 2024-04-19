#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <unistd.h>

#define BACKLOG 10
#define SERVPORT 12345
#define COUNT 5
int socket_fd[COUNT];

void *pthread_function(int client_fd)
{
    char message[1500];
    char buf[1024];
    char name[20];

    int i;
    int recvbytes;

    // 保存客户端名字
    recvbytes = recv(client_fd, name, 20, 0);
    name[recvbytes] = ':';
    name[recvbytes + 1] = '\0';

    while (1)
    {
        if ((recvbytes = recv(client_fd, buf, 1024, 0)) == 0)
        {
            printf("%s sbye!\n", name);
            break;
        }
        buf[recvbytes] = '\0';
        for (i = 0; i < COUNT; i++)
        {
            if (socket_fd[i] == -1)
            {
                continue;
            }
            else
            {
                message[0] = '\0';
                strcat(message, name);
                strcat(message, buf);
                if (send(socket_fd[i], message, strlen(message), 0) == -1)
                {
                    perror("send error");
                    exit(1);
                }
                printf("%s\n",message);
            }
        }
        fflush(stdout);
    }
    close(client_fd);
    for (i = 0; i < COUNT; i++)
    {
        if (socket_fd[i] == client_fd)
        {
            socket_fd[i] = -1;
        }
    }
    pthread_exit(NULL);
}

int main()
{
    int i;
    for (i = 0; i < COUNT; i++)
    {
        socket_fd[i] = -1;
    }
    pthread_t id;
    int sockfd;
    int clientfd;

    socklen_t sin_size;

    struct sockaddr_in remote_addr;
    struct sockaddr_in my_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    __bzero(&my_addr.sin_zero, 8);

    bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));

    listen(sockfd, BACKLOG);

    i = 0;
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);

        clientfd = accept(sockfd,(struct sockaddr*)&remote_addr,&sin_size);

        while (socket_fd[i] != -1)
        {
            i = (i+1)%COUNT;
        }
        socket_fd[i] = clientfd;
        pthread_create(&id,NULL,(void*)pthread_function,(int*)clientfd);
    }
}