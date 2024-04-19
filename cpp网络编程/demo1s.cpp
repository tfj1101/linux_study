#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
using namespace std;
#define MAXLINE 80
#define SERV_PORT 8000
int main(int agrc,char* argv[])
{

        int listenfd,connfd;
        socklen_t cliaddr_len;
        char buf[MAXLINE];
        char str[INET_ADDRSTRLEN];
        int i,n;
        listenfd = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in cliaddr;
        struct sockaddr_in servaddr;
        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        servaddr.sin_addr.s_addr = htons(INADDR_ANY);
        bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        listen(listenfd,10);
        cout<<"Accepting connections..."<<endl;
        while(true)
        {
                cliaddr_len = sizeof(cliaddr);
                connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddr_len);
                n = read(connfd,buf,MAXLINE);
                cout<<"received from "<<inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str))<<"at port "<<ntohs(cliaddr.sin_port)<<endl;
                for(i = 0;i<n;i++)
                {
                        buf[i] = toupper(buf[i]);
                }
                write(connfd,buf,n);
                close(connfd);
        }

}

