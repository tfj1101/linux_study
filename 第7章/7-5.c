#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
void fun_ctrl_c();

int main()
{
	int i;
	struct sigaction action;
	sigset_t set,pendset;
	(void) signal(SIGINT,fun_ctrl_c);
	if(sigemptyset(&set)<0)
	{
		perror("chu shi hua shi bai failed");
	}
	if(sigaddset(&set,SIGINT)<0)
	{		
		perror("add SIGINT failed");
	}
	if(sigprocmask(SIG_BLOCK,&set,NULL))
	{
		perror("failed");
	}
	else
	{
		for(i = 1;i<=6;i++)
		{
			printf("xian zai biao shi zu se zhuang tai\n");
			sleep(2);
		}
	}
	if(sigprocmask(SIG_UNBLOCK,&set,NULL)<0)
	{
		perror("failed!");
	}
	
	return 0;
}

void fun_ctrl_c()
{
	printf("\t ning an le ctrl_c wo:\n");
	(void) signal(SIGINT,SIG_DFL);
}
