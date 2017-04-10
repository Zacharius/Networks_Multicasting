//Group 9 Project 2a

#include <sys/types.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#define PORT 22209
#define GROUP "239.10.5.9"

int main (int argc, char * argv[]) {
	
	struct sockaddr_in server;
	int sockfd, n, count = 0;
	int ttl = 30;
	char message[50];
	
	int reuse = 1; 
	
	//create a socket and error check
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//check if socket was properly created
	if (sockfd < 0) {
		printf("Socket error = %d\n", sockfd);
		return (-1);
	}
	//allow socket to be reused if it's still waiting on old process
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
       printf("unable to reuse socket");
       exit(1);
	}
	setsockopt(sockfd,IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
	
	//set address structure
	/*fill in my address and port*/
	memset(&server, '0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr(GROUP);
	
	
	while (1) {
		count++;
		sprintf(message, "This is message [%d] from the Group 9 beacon\n", count);
		if (sendto(sockfd,message,sizeof(message),0,(struct sockaddr *) &server, sizeof(server)) < 0) {
	       perror("sendto");
	       exit(1);
	  }
	  printf("%s", message);
	  sleep(1);
	}
}