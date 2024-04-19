#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int main()
{
	pid_t result;
	ssize_t r_num;
	int pipe_fd[2];
	char buf_r[100],buf_w[100];
	memset(buf_r,0,sizeof(buf_r));
	if(pipe(pipe_fd)<0)
	{
		printf("create failed!");
		return -1;
	}
	result = fork();
	if(result<0)
	{
		printf("create pid failed");
		exit(0);
	}
	else if(result == 0)
	{
		close(pipe_fd[1]);
		if((r_num = read(pipe_fd[0],buf_r,100))>0)
		{
			printf("the number is %d ge char,string is: %s\n",r_num,buf_r);
		}
		close(pipe_fd[0]);
		exit(0); 	
	}
	else
	{
		close(pipe_fd[0]);
		printf("qing shu ru string:\n");
		scanf("%s",buf_w);
		if(write(pipe_fd[1],buf_w,strlen(buf_w))!=-1)
		{
			printf("xie ru de string is %s\n",buf_w);
		}
		close(pipe_fd[1]);
		waitpid(result,NULL,0);
		exit(0); 
	}
	 
}
