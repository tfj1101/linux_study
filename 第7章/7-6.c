#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
void fun_ctrl_c();
void fun_ctrl_z();
void fun_ctrl_d();
int main()
{
	int i;
	struct sigaction action;
	sigset_t set,pendset;
	(void) signal(SIGINT,fun_ctrl_c);
	(void) signal(SIGTSTP,fun_ctrl_z);
	(void) signal(SIGQUIT,fun_ctrl_d);
	if(sigemptyset(&set)<0)
	{
		perror("chu shi hua shi bai failed");
	}
	if(sigaddset(&set,SIGINT)<0)
	{		
		perror("add SIGINT failed");
	}
	if(sigaddset(&set,SIGTSTP)<0)
	{		
		perror("add SIGTSTP failed");
	}
	if(sigprocmask(SIG_BLOCK,&set,NULL))
	{
		perror("failed");
	}
	else
	{
		for(i = 1;i<=6;i++)
		{
			printf("xian zai <Ctrl_c> <Ctrl_z> biao shi zu se zhuang tai\n");
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
	int i;
	printf("\t ning an le ctrl_c wo:\n");
	for(i=1;i<=5;i++)
	printf("zheng zai chu li ctrl_z sign\n");
}
void fun_ctrl_z()
{
	int i;
	printf("\t ning an le ctrl_z wo:\n");
	for(i=1;i<3;i++)
	{
		printf("zheng zai chu li ctrl_z sign\n");	
	}
}
void fun_ctrl_d()
{
	int i;
	printf("\t ning an le ctrl_d wo:\n");
	for(i=1;i<3;i++)
	{
		printf("zheng zai chu li ctrl_d sign\n");	
	}
}
