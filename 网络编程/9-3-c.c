#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
int port = 8888;
int main()
{
    int sockfd;
    int i = 0;
    int z;
    char buf[80];
    char strl[80];
    struct sockaddr_in adr_server;
    FILE *fp;
    printf("打开文件...\n");
    fp = fopen("liu", "r");
    printf("连接服务器...\n");
    // 建立IP
    adr_server.sin_family = AF_INET;
    adr_server.sin_port = htons(port);
    adr_server.sin_addr.s_addr = htonl(INADDR_ANY);
    __bzero(&(adr_server.sin_zero), 8);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("发送文件...\n");
    for (i = 0; i < 3; i++)
    {
        fgets(strl, 80, fp);
        printf("%d :%s", i, strl);
        sprintf(buf, "%d:%s", i, strl);
        z = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&adr_server, sizeof(adr_server));
    }
    printf("发送.....\n");
    sprintf(buf, "stop\n");
    printf("sockfd:%d\n",sockfd);
    z = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&adr_server, sizeof(adr_server));
    fclose(fp);
    close(sockfd);
    exit(0);
}