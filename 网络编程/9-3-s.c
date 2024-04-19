#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
int port = 8888;
int main()
{
    int sockfd;
    int len;
    int z;
    char buf[256];
    struct sockaddr_in adr_inet;
    struct sockaddr_in adr_clet;
    printf("等待客户端....\n");
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(port);
    adr_inet.sin_addr.s_addr = htonl(INADDR_ANY);
    __bzero(&(adr_inet.sin_zero),8);
    len = sizeof(adr_clet);
    //建立socket
    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    //绑定socket
    bind(sockfd,(struct sockaddr *)&adr_inet,sizeof(adr_inet));

    while (1)
    {
        z=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&adr_clet,&len);
        buf[z] = '\0';
        printf("接收：%s",buf);
        printf("sockfd: %d\n",sockfd);
        if(strncmp(buf,"stop",4)==0)
        {
            printf("结束...");
            break;
        }
    }
    close(sockfd);
    exit(0);
}