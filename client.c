//Zachary Faddis, xrn665
#include <sys/types.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <time.h>
#include <stdio.h>

#define TARGET_PORT 22209
#define TARGET_ADDR "239.10.5.9"

void error(int ret, char *msg);

int main()
{

  int sock, i, addrlen, size;
  int reuse = 1;
  struct sockaddr_in client;
  struct ip_mreq mreq;
  char *time_string, msg[50];
  time_t t;
  
  t = time(NULL);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0)
  {
    printf("Socket error = %s\n", strerror(errno));
    exit(-1);
  }

  memset(&client, 0, sizeof(client));
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = htonl(INADDR_ANY);
  client.sin_port = htons(TARGET_PORT);

  i = bind(sock, (struct sockaddr *) &client, sizeof(client) );
  if(i<0)
  {
    printf("bind error = %s\n", strerror(errno));
    exit(-1);
  }
  /* allow multiple sockets to use the same PORT number */
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse)) < 0) {
       error(-1, "unable to reuse socket");
       }
   
	/* use setsockopt() to request that the kernel join a multicast group */
	mreq.imr_multiaddr.s_addr=inet_addr(TARGET_ADDR);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));

  while(1)
  {
	memset(msg, 0, sizeof(msg));
	addrlen = sizeof(client);
    size = recvfrom(sock, msg, 1024, 0, (struct sockaddr *) &client, &addrlen);
    if (size < 0) {
			perror("No message received\n");
		} else
			time_string = strtok(ctime(&t), "\n");
			printf("%s - %s", time_string, msg);
			t = time(NULL);
  }
  close(sock);
  exit(0);
}

void error(int ret, char *msg)
{
  if(ret<1)
  {
    printf("%s\n", msg);
    printf("program exiting\n");
    exit(-1);
  }
}