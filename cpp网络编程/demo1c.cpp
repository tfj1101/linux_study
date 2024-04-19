#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<netdb.h>
#include<mysql/mysql.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
using namespace std;

int main(int agrc,char* argv[])
{	
	if(agrc!=3)
	{
		cout<<"Using :/demo1 服务端IP 服务端端口\nExampe:./demo1c 192.168.200.130 8000\n"<<endl;
		return -1;
	}
	//第一步：创建客户端socke
	int sockfd;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	cout<<"socket = "<<sockfd<<endl;	
	//第2步：向服务器发送连接请求。
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));

	//获取IP地址的方法、
	//第一种	可域名，可ip
	struct hostent* h;
	if((h = gethostbyname(argv[1]))==NULL)
	{	
		cout<<"gethostbyname failed."<<endl;
		close(sockfd);
		return -1;
	}
	memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);

	//第二种	只可ip -----inet_addr(char* ip)
	//servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	//
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr))==-1)
	{
		perror("connect");
		close(sockfd);
		return -1;
	}

	//第三步：与服务端通讯
	char buf[1024];
	for(int i = 0;i<10;i++)
	{
		int iret;
		memset(buf,0,sizeof(buf));
		sprintf(buf,"这是第%d个,编号%03d.",i+1,i+1);
		if((iret = send(sockfd,buf,strlen(buf),0))<=0)
		{
			perror("send");break;
		}

	
		cout<<"发送："<<buf<<endl;
		memset(buf,0,sizeof(buf));
		if((iret = recv(sockfd,buf,sizeof(buf),0))<=0)
		{
			cout<<"iret = "<<iret<<endl;break;
		}
 		cout<<"接收："<<buf<<endl;
		sleep(1);
	}
	close(sockfd);	
}
