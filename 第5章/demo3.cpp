#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int fd;
	char buf[1024];
	fd = open("data.txt",O_CREAT | O_RDWR | O_TRUNC);
	if(fd == -1)
	{
		perror("open(data.txt)");
		exit(1);
	}
	printf("文件描述符fd=%d\n",fd);
	strcpy(buf,"我是一只啥啥鸟。\n");
	if(write(fd,buf,strlen(buf))==-1)
	{
		perror("write()");
		exit(1);
	}
	close(fd);
}