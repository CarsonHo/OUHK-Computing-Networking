#include  <stdio.h>
#include  <string.h>
#include  <sys/wait.h>
#define  BUF_SIZE  500

main ()
{
int pid;
int status=0;
char buf[BUF_SIZE];

pid=fork();
switch(pid) 
{
case -1:
// fork failure
		sprintf(buf, "process fork failure....");
		write(1, buf, strlen(buf));
		break;
case 0:
//code for child process
	 {
	 char *args[2];
	 args[0] = "ps";
	 args[1] = "\0";
		sprintf(buf, "child: my pid = %d,  parent pid = %d\n", getpid(),  getppid());
		write(1, buf, strlen(buf));
		sleep(2);
		sprintf(buf, "child: I am now running command ps \n");
		write(1, buf, strlen(buf));
		execvp(args[0], args);
		exit(status);
		}
default:
		{
		//code for parent process
		sprintf(buf, "parent: my pid = %d and my child process pid = %d\n", getpid(), pid);
		write(1, buf, strlen(buf));
		
		sprintf(buf, "parent : waiting for the exit of the child\n");
		write(1, buf, strlen(buf));
		
		wait(&status);
	   sprintf(buf, "parent: my child is exited with status %d and I exit now\n", WEXITSTATUS(status));
		write(1, buf, strlen(buf));
		
		}
}
}
