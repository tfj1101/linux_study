#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
	pid_t pc,pr; 
	pc=fork();
	if(pc<0)
		printf("failed!");
	else
	{
		if(pc == 0)
		printf("child:%d",getpid());
		else
		{
			sleep(20);
			printf("The parent:%d",getpid());
		}
	}
	exit(0);
}
