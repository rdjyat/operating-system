#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	//create process
	pid_t pid;
	pid=fork();
	if (pid < 0)
	{
		perror("fork");
	}
	if (pid == 0)
	{
		//son process
		pid = execl ("/usr/bin/vi","vi", NULL);
	}
	else if(pid >0)
	{
		//father process
		while(1){};
	}
	return 0;
}
