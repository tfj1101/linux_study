#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	pid_t pid;
	char* message;
	int n;
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(0);
	}
	if (pid == 0)
	{
		message = "This is the child\n";
		n = 3;
	}
	else
	{
		message = "This is the parent\n";
		n = 6;

	}
	for (; n > 0; n--)
	{
		printf(message);
		sleep(1);
	}
	return 0;
}
