#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "./client 192.168.200.130 5050" << endl;
        return -1;
    }

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout<<"socket failed"<<endl;
        close(sockfd);
        return -1;
    }

    sockaddr_in seraddr;
    memset(&seraddr,0,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(atoi(argv[2]));
    seraddr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sockfd,(sockaddr*)&seraddr,sizeof(seraddr))==-1)
    {
        cout<<"connect failed"<<endl;
        close(sockfd);
        return -1;
    }   
    cout<<"connect ok"<<endl;
    char buf[1024];
    for (size_t i = 0; i < 10; i++)
    {
        memset(buf,0,sizeof(buf));
        cout<<"请输入要发送的信息；"<<endl;
        cin>>buf;
        if (send(sockfd,buf,sizeof(buf),0)<=0)
        {
            cout<<"write() failed"<<endl;
            close(sockfd);
            return -1;
        }
        memset(buf,0,sizeof(buf));
        if (recv(sockfd,buf,sizeof(buf),0)<=0)
        {
            cout<<"read failed"<<endl;
            close(sockfd);
            return -1;
        }
        
        cout<<"recv :"<<buf<<endl;
    }
    
}