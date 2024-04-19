#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{

    int sockfd, recvbytes;
    char buf[1024];
    struct hostent *host;
    struct sockaddr_in serv_addr;
    if (argc < 2)
    {
        fprintf(stderr, "please enter the server's hostname\n");
        exit(1);
    }
    if ((host = gethostbyname(argv[1])) == NULL)
    {
        herror("get fail");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket create fail!");
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    memcpy(&serv_addr.sin_addr, host->h_addr, host->h_length);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect error");
        exit(1);
    }
    cin>>buf;
    if (send(sockfd,buf,1024,0)==-1)
    {
        cout<<"send failed"<<endl;
        exit(1);
    }
    if ((recvbytes = recv(sockfd, buf, 1024, 0)) == -1)
    {
        perror("connect 出错");
        exit(1);
    }
    cout << "收到：" << buf << endl;
    close(sockfd);
    return 0;
}
