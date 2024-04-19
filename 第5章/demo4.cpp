#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
int main()
{
	int fd;
	char buf[1024];
	fd = open("data.txt",O_RDONLY);
	if(fd == -1)
	{
		cout<<"open failed!";
		exit(1);
	}
	cout<<"文件描述符fd="<<fd<<endl;
	memset(buf,0,sizeof(buf));
	if(read(fd,buf,sizeof(buf))==-1)
	{
		cout<<"read failed";
		exit(1);
	}
	cout<<buf<<endl;
	close(fd);
}
