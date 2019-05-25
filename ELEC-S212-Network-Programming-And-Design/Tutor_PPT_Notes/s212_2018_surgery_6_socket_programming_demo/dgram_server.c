/*
** dgram_server.c -- a datagram sockets "server". Without forking 
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

#define SERVER_PORT 3490    // the port clients will be connecting to

#define MAXBUFLEN 100

int main(void)
{
	   //int pid, status=0;
    int sockfd;
    struct sockaddr_in srv_info;    // server's address information
    struct sockaddr_in clnt_info; // client's address information
    int addr_len, numbytes;
    char buf[MAXBUFLEN];

//Step 1: socket creation
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		{
        perror("socket creation error");
        exit(1);
    }
		printf("Socket %d is created for Datagram socket server\n", sockfd);

//Step 2: Prepare for binding to server's IP address and port number
//  &     bind() to created socket
    srv_info.sin_family = AF_INET;         // host byte order
    srv_info.sin_port = htons(SERVER_PORT);     // short, network byte order
    srv_info.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(srv_info.sin_zero), '\0', 8); // zero the rest of the struct
    if (bind(sockfd, (struct sockaddr *)&srv_info, sizeof(struct sockaddr)) == -1) 
		{
        perror("bind error");
        exit(1);
    }
		printf("socket %d is binded to server port= %d \n\n", sockfd, ntohs(srv_info.sin_port));

	//Step 3: Data transfer

	//receive data from client
	while (1)
	{
		addr_len = sizeof(struct sockaddr);
    if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&clnt_info, &addr_len)) == -1) 
		{
        perror("recvfrom error");
        exit(1);
    }

    buf[numbytes] = '\0';
    
		printf("Rx (from client of IP= %s & port= %d) : \"%s\"\n",inet_ntoa(clnt_info.sin_addr), ntohs(clnt_info.sin_port), buf);

		//Transmit data to client 
		if ((numbytes=sendto(sockfd, "Your message is received\n", 26, 0, (struct sockaddr *)&clnt_info, sizeof(struct sockaddr))) == -1) 
		{
			perror("sendto error");
			exit(1);
    }
	   printf("Tx (to client of IP= %s & port= %d) : \"%s\"\n",inet_ntoa(clnt_info.sin_addr), ntohs(clnt_info.sin_port), "Your message is received");
	   printf("Type ctrl-c  to exit server\n\n");
 } //end while loop

 close(sockfd);

	return 0;
}
