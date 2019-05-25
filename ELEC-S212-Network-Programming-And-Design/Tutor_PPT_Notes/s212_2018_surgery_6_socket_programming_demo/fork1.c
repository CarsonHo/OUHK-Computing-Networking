#include  <stdio.h>
#include  <string.h>
#include  <sys/wait.h>
#define  BUF_SIZE   500

main ()
{
int pid, i;
int common_var=10;
int status=0;
char  buf[BUF_SIZE];

pid=fork();
switch(pid) 
{
case -1:
// fork failure
		printf("process fork failure....");
		exit(0);

case 0:
//code for child process
		sprintf(buf, "child: my pid = %d, my parent pid = %d\n", getpid(),  getppid());
		write(1, buf, strlen(buf));

		for (i=1; i<=5; i++)
			{
				sprintf(buf, "child:  running at i = %d\n", i);
				write(1, buf, strlen(buf));
				sleep(1);
			}
		sprintf(buf, "child: I exit now\n");
		write(1, buf, strlen(buf));

		exit(status);

default:
//code for parent process,
// if pid is positive, it is the process id of child process
			sprintf(buf, "parent: my pid = %d, waiting for exit of my child process %d\n", getpid(), pid);
			write(1, buf, strlen(buf));
		   
			wait(&status);  // parent wait for end of child process
			
			sprintf(buf, "parent: my child is exited with status %d and I exit now\n", WEXITSTATUS(status));
			write(1, buf, strlen(buf));
}
}
