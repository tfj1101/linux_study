#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
int main()
{
	pid_t result;
	int ret;
	result = fork();
	int newret;
	if(result<0)
	{
		perror("create failed!");
	}
	else if(result == 0)
	{
		raise(SIGKILL);
		exit(0);
	}
	else
	{
		printf("child pid is:%d\n",result);
		if((waitpid(result,NULL,WNOHANG))==0)
		{
			if(ret = kill(result,SIGKILL)==0)
			{
				printf("ret = %d,pid = %d\n",ret,result);
				
			}
			else
			{
				perror("kill failed");
			}
		}
	}
	return 0;
}
