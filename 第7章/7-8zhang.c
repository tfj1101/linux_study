#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/select.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int i,rfd,wfd,len=0,fd_in;
	char str[32]; 
	int flag,stdinflag;
	fd_set write_fd,read_fd;
	struct timeval net_timer;
	mkfifo("fifo1",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	mkfifo("fifo2",S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	wfd = open("fifo2",O_WRONLY);
	rfd = open("fifo1",O_RDONLY);
	if(wfd<=0 || rfd<=0)
		//return 0;
	printf("li si de termial\n");
	while(1)
	{
		FD_ZERO(&read_fd);
		FD_SET(rfd,&read_fd);
		FD_SET(fileno(stdin),&read_fd);
		net_timer.tv_sec = 5;
		net_timer.tv_usec = 0;
		memset(str,0,sizeof(str));
		if((i = select(rfd+1,&read_fd,NULL,NULL,&net_timer))<=0)
		{
			continue;
		}
		if(FD_ISSET(rfd,&read_fd))
		{

			read(rfd,str,sizeof(str));
			printf("---------------------------\n");
			printf("zhangsan:%s\n",str);
			
		}
		if(FD_ISSET(fileno(stdin),&read_fd))
		{
			printf("----------------------\n");
			fgets(str,sizeof(str),stdin);
			len = write(wfd,str,strlen(str));
		}
		
	}
	close(rfd);
	close(wfd);
	
}
