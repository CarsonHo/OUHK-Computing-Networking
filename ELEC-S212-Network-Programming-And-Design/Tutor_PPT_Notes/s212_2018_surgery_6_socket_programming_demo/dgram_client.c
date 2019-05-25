/*
** dgram_client.c -- a datagram socket "client" 
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
#include <netdb.h>

#define SERVER_PORT 3490    // the port users will be connecting to
#define MAXBUFLEN 100

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in clnt_info; // client's address information
    struct sockaddr_in clnt_info2; // client's address information
    struct sockaddr_in srv_info; // server's address information
    struct hostent *srv_name;   
    int numbytes, addr_len;
	   char buf[MAXBUFLEN];

    if (argc != 3) {
        fprintf(stderr,"usage: dgram_client dns_name_of_server  message_to_server\n");
        exit(1);
    }

    if ((srv_name=gethostbyname(argv[1])) == NULL) {  // get the host info
        perror("gethostbyname");
        exit(1);
    }

//Step 1: socket creation
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
		printf("Socket %d is created for Datagram socket client\n", sockfd);

//Step 2: Prepare for binding to client's IP & any unused port 
//        bind() to created socket
		clnt_info.sin_family = AF_INET;     // host byte order
		clnt_info.sin_addr.s_addr = INADDR_ANY; // automatically fill with client's IP
    clnt_info.sin_port = htons(0); // bind to any unused port, short, network byte order
    memset(&(clnt_info.sin_zero), '\0', 8);  // zero the rest of the struct
	 	if (bind(sockfd, (struct sockaddr *)&clnt_info, sizeof(struct sockaddr)) == -1) 
		{
			perror("bind error");
			exit(1);
		}	
		printf("Socket %d is binded to any unused local port \n", sockfd);


		
//Step 3: Prepare for server's IP & port info
		srv_info.sin_family = AF_INET;     // host byte order
    srv_info.sin_port = htons(SERVER_PORT); // short, network byte order
    srv_info.sin_addr = *((struct in_addr *)srv_name->h_addr);
    memset(&(srv_info.sin_zero), '\0', 8);  // zero the rest of the struct

//Note that connect() is not needed for connectionless client
//Step 4: data transfer with server
	
	   //Transmit data to server
		if ((numbytes=sendto(sockfd, argv[2], strlen(argv[2]), 0,
		     (struct sockaddr *)&srv_info, sizeof(struct sockaddr))) == -1) {
        perror("sendto error");
        exit(1);
    }
    printf("Tx (to server of IP= %s & port= %d): \"%s\"\n", inet_ntoa(srv_info.sin_addr), ntohs(srv_info.sin_port), argv[2]);
		
		//Rx data from server
		addr_len = sizeof(struct sockaddr);
	   if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&srv_info, &addr_len)) == -1) {
		   perror("recvfrom error");
			exit(1);
		}
	   buf[numbytes] = '\0';
		printf("Rx (from server of IP= %s & port= %d) : \"%s\"\n",inet_ntoa(srv_info.sin_addr), ntohs(srv_info.sin_port), buf);
    
	   close(sockfd);

    return 0;
}
