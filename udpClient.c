//Zachary Faddis, xrn665
#include <sys/types.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#define TARGET_PORT 20008
#define TARGET_ADDR "67.11.114.117"
#define MY_IP "127.0.0.1"

#define GROUP     joinGroup.imr_multiaddr.s_addr
#define INTERFACE joinGroup.imr_interface.s_addr

void error(int ret, char *msg);

int main()
{

  int sock, i;
  struct sockaddr_in toPC, fromPC;
  struct ip_mreq joinGroup;
  char msg[50];

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  if(sock < 0)
  {
    printf("Socket error = %s\n", strerror(errno));
    exit(-1);
  }
  
  GROUP = inet_addr(TARGET_ADDR);
  INTERFACE = inet_addr(MY_IP);

  i = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &joinGroup, 
                  sizeof(struct ip_mreq));
  error(i, "problem joing multicast group"); 

  toPC.sin_family = AF_INET;
  toPC.sin_port = htons(TARGET_PORT);
  toPC.sin_addr.s_addr = inet_addr(TARGET_ADDR);

  fromPC.sin_family = PF_INET;
  fromPC.sin_port = htons(0);
  fromPC.sin_addr.s_addr = htonl(INADDR_ANY);

  i = bind(sock, (struct sockaddr *) &fromPC, sizeof(fromPC) );
  if(i<0)
  {
    printf("bind error = %s\n", strerror(errno));
    exit(-1);
  }

  while(1==1)
  {
    recvfrom(sock, &msg, 100, 0, &toPC, sizeof(struct sockaddr_in));
    printf("%s\n", msg);
  }
  
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
