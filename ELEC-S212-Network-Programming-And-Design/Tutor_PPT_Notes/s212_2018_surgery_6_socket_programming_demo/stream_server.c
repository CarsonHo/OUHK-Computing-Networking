/*
** stream_server.c -- a stream socket server, listening on SERVER_PORT=3490  and forks a child process to service client request
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXDATASIZE 500 // max number of bytes we can get at once
#define SERVER_PORT 3490    // The server socket will bind to SERVER_PORT

#define BACKLOG 10     // size of  pending connections queue is BACKLOG 


int main(void)
{
  	int pid, status=0;
    int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_sockfd
    struct sockaddr_in srv_info;    // server's address information
    struct sockaddr_in clnt_info; // client's address information

		int  numbytes ;
		char buf[MAXDATASIZE];
		char out_buf[MAXDATASIZE];

    int sin_size;
	 
	   int yes=1;

//Step 1: create a stream socket on INTERNET domain, 
//3rd agrument = 0 means let system to select protocol based on AF_INET and SOCK_STREAM.  You can directly use TCP.
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{
        perror("socket creation error");
        exit(1);
    }
		printf("socket %d is created for stream socket server\n", sockfd);


//Step 2: prepare value of field member (family type, port number, IP address) of srv_info and then bind()
    srv_info.sin_family = AF_INET;         // host byte order
    srv_info.sin_port = htons(SERVER_PORT);     // short, network byte order
    srv_info.sin_addr.s_addr = INADDR_ANY; // automatically fill with server's IP
    memset(&(srv_info.sin_zero), '\0', 8); // zero the rest of the struct

//        bind() the sockfd to the values of field members of srv_info
    if (bind(sockfd, (struct sockaddr *)&srv_info, sizeof(struct sockaddr)) == -1) 
		{
        perror("bind error");
        exit(1);
    }
		printf("socket %d is binded to server port= %d \n", sockfd, ntohs(srv_info.sin_port));

//Step 3: listen to the created socket (sockfd) and set queue length to BACKLOG
    if (listen(sockfd, BACKLOG) == -1) 
		{
        perror("listen error");
        exit(1);
    }

		printf("Listening socket %d. Waiting for client request\n",sockfd);


//Step 4: While loop to accept() client requests : 
//For each client connection request, accept() generates a new socket (sockfd).
//A child process is  forked and use new_sockfd to communicate with client. 
    
		while(1)   // parent process always loops to wait for new client connect.
		{ 
        sin_size = sizeof(struct sockaddr_in);
			
        if ((new_sockfd = accept(sockfd, (struct sockaddr *)&clnt_info, &sin_size)) == -1) 
			 {
            perror("accept error");
            continue;
        }
       
			 printf("\n**********************************************************\n");
			 printf("Got connection from client(IP= %s & port= %d). I am forking a child. My child communicates with this client using new socket %d\n\n",inet_ntoa(clnt_info.sin_addr), ntohs(clnt_info.sin_port), new_sockfd);
        
		    pid=fork();  // fork a child process to service client connection

			 switch(pid)
			 {
			    case -1: 
			    {
              perror("fork error");
              exit(1);
           }

			    case 0: 	  // this is the child process
			    {
					 printf("================================================\n");
              close(sockfd); // child doesn't need the socket of  listener
				 
					 printf("Child: I am forked to service new client request. My pid = %d,  parent pid = %d\n", getpid(),  getppid());

				    //receive data from client 
				  	 if (read(new_sockfd, buf, MAXDATASIZE-1) == -1)
					 {
					   perror("read error");
					   exit(1);
					 }
				     printf("Child: I Rx from client IP= %s & port= %d --> \"%s\"\n",inet_ntoa(clnt_info.sin_addr) , ntohs(clnt_info.sin_port), buf);

				    //send data to client
              if (write(new_sockfd, "Your message is received\n", 27) == -1)  
				    {
					   perror("write error");
					   exit(1);
					 }
              printf("Child: I Tx to client IP= %s & port= %d --> \"Your message is received\"\n", inet_ntoa(clnt_info.sin_addr), ntohs(clnt_info.sin_port));


              printf("Child: I have serviced my client and now close new socket %d. I quit now \n", new_sockfd);
              close(new_sockfd);
              exit(status);
           }  //end case 0

			    default:  //parent process
				 {
					 printf("================================================\n");
					 printf("Parent: my pid = %d and my child process pid = %d\n", getpid(), pid);
			       close(new_sockfd);  // parent doesn't need this 
				    printf("Parent: I have closed new_sockfd %d. I am waiting for new client connection. Prese ctrl-c to quit me\n", new_sockfd);
					 wait(&status);
           }  //end default case

       } //end switch
 
	   } // end while
    return 0;
}

