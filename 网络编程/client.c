#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>

#define SERVPORT 4444

char send_buf[1024];
char recv_buf[1500];

void* pthread_funtion(int sockfd)
{
    int recvbytes;
    while (1)
    {
        recvbytes = recv(sockfd,recv_buf,1500,0);
        recv_buf[recvbytes] = '\0';
        printf("收到：%s\n",recv_buf);
        fflush(stdout);
    }
}

int main(void)
{
    pthread_t id;
    int sockfd;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVPORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.200.130");
    __bzero(&serv_addr.sin_zero,8);

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr));

    char name[20];
    printf("input your name:");
    scanf("%s",name);
    
    send(sockfd,name,strlen(name),0);

    pthread_create(&id,NULL,(void*)pthread_funtion,(int*)sockfd);

    while (1)
    {
        fgets(send_buf,sizeof(send_buf),stdin);

        send(sockfd,send_buf,strlen(send_buf),0);

        sleep(1);
    }
    close(sockfd);
    pthread_cancel(id);
    return 0;
    
}