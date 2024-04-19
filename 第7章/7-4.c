#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
void fun_ctrl_c();
void fun_ctrl_c()
{
	printf("\t ning an le ctrl_c wo:\n");
	(void) signal(SIGINT,SIG_DFL);
}
int main()
{
	(void) signal(SIGINT,fun_ctrl_c);
	printf("zhuchengxujinruwuxianxunhuang!\n");
	while(1)
	{
		printf("this is a wu xian xun huang!\n");
		sleep(3);
	}
	exit(0);
}
