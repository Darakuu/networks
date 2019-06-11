// UDP Server Bidirezionale
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char* fgets_n(char *msg, size_t msglen, FILE *keyinput)
{
	if(fgets(msg,msglen,keyinput) != 0)
	{
		msg[ strcspn(msg,"\n") ] = '\0';
		return msg;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int sockfd,n;
	struct sockaddr_in local_addr, remote_addr;
	socklen_t len = sizeof(remote_addr);
	char msg[1000];

	if (argc<2)
	{
		printf("USA: ./UDP_Server <PORTA>\n");
		return 0;
	}

	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		printf("Errore di socket binding\n");
		return -1;
	}

	memset(&local_addr,0,sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(atoi(argv[1]));
	bind(sockfd, (struct sockaddr *) &local_addr, sizeof(local_addr));
	
	printf("Listening on [%d]...\n",ntohs(local_addr.sin_port));
	for (;;)
	{
		n = recvfrom(sockfd, msg,1000,0,(struct sockaddr *) &remote_addr, &len);
		msg[n] = 0;		
		printf("From [%s:%d]: \"%s\"\n",inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port),msg);
		sendto(sockfd,msg,n,0,(struct sockaddr *) &remote_addr, len);
		
	}
}
