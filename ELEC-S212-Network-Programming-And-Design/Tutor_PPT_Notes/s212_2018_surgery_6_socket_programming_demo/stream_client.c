/*
** stream_client.c -- a stream socket client
** syntax :  stream_client   dns_name_of_server  message_to_server
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>


#define MAXDATASIZE 300 // max number of bytes we can get at once 
#define SERVER_PORT 3490

int main(int argc, char *argv[])
{
    int sockfd, numbytes ;  
    char buf[MAXDATASIZE];
    struct hostent *srv_name;
    struct sockaddr_in srv_info; // server's address information 
    struct sockaddr_in clnt_info; // client's address information 

    if (argc != 3) {
        fprintf(stderr,"usage: stream_client dns_name_of_server message_to_server\n");
        exit(1);
    }

    if ((srv_name=gethostbyname(argv[1])) == NULL) {  // get the host info 
        perror("gethostbyname error");
       exit(1);
   }

//Step 1: create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket creation error");
        exit(1);
    }
	printf("socket %d is created for stream socket client\n",sockfd); 

//prepare for connect()
    srv_info.sin_family = AF_INET;    // host byte order 
    srv_info.sin_port = htons(SERVER_PORT);  // short, network byte order 
    srv_info.sin_addr = *((struct in_addr *)srv_name->h_addr);
    memset(&(srv_info.sin_zero), '\0', 8);  // zero the rest of the struct 



//Step 2: connnet() to remote server
    if (connect(sockfd, (struct sockaddr *)&srv_info, sizeof(struct sockaddr)) == -1) {
        perror("connect error");
        exit(1);
    }


//get client's information by getsockname()  & stored in struct sockaddr clnt_info
//debug        
//		getsockname(sockfd, (struct sockaddr*)&clnt_info, (int *)sizeof(clnt_info));

//		printf("socket %d, connects  to remote server IP= %s & port= %d \n",sockfd, inet_ntoa(srv_info.sin_addr), ntohs(srv_info.sin_port));


//Step 3: Data transfer with server
		//write data to serve
		 if (write(sockfd, argv[2], MAXDATASIZE-1) == -1) {
        perror("write error");
        exit(1);
    }

            printf("Tx: writing data %s to server\n", argv[2]);
//debug        
//            printf("Tx (to server IP= %s & port= %d): \"%s\"\n", inet_ntoa(srv_info.sin_addr), ntohs(srv_info.sin_port), argv[2]);


    //read data from server
	   if (read(sockfd, buf, MAXDATASIZE-1) == -1) {
        perror("read error");
        exit(1);
    }
    printf("Rx: reading data %s from server \n",  buf);

    //debug
   // printf("Rx (from server IP= %s & port= %d): \"%s\"\n", inet_ntoa(srv_info.sin_addr), ntohs(srv_info.sin_port), buf);

    close(sockfd);

    return 0;
}

