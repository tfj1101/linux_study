#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define SERVPORT 8888
#define MAXDATASIZE 100

int main(int argc,char* argv[])
{
    int sockfd,recvbytes;
    char buf[MAXDATASIZE];
    struct hostent* host;
    struct sockaddr_in serv_addr;
    if(argc<2)
    {
        fprintf(stderr,"please entere the server's hostname!\n");
        exit(1);
    }
    if ((host = gethostbyname(argv[1]))==NULL)
    {
        printf("gethostbyname 失败！\n");
        exit(1);
    }
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket 失败！\n");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr,host->h_addr_list[0],host->h_length);
    //serv_addr.sin_addr = *((struct in_addr*)host->h_addr_list[0]);
    serv_addr.sin_port = htons(SERVPORT);
    memset(&(serv_addr.sin_zero),0,8);
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect 失败！\n");
        exit(1);
    }
    memset(&buf,0,MAXDATASIZE);
    if(recvbytes = recv(sockfd,buf,MAXDATASIZE,0)==-1)
    {
        perror("connect 失败！\n");
        exit(1);

    } 
    printf("recvbytes = %d\n",recvbytes);
    buf[recvbytes] = '\0';
    printf("收到：%s\n",buf);
    close(sockfd);
}