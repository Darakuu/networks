// UDP Client con ricezione Broadcast
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* fgets_noNewline(char *msg, size_t msglen, FILE *keyinput)
{
	if (fgets(msg,msglen,keyinput) != 0)
	{
		msg[strcspn(msg,"\n")] = '\0';
		return msg;
	}
	return NULL;
}


int main(int argc, char*argv[])
{
	int sockfd,sockbd,n, broadcastperm=1;
	struct sockaddr_in remote_addr, local_addr, broadcast_addr;
	char sendline[1000];
	char recvline[1000];
	socklen_t len = sizeof(remote_addr);
	socklen_t blen = sizeof(broadcast_addr);

	if(argc!=3)
	{
		printf("USA: UDP_ClientUser <IP> <PORTA>\n");
		return 1;
	}

	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("\nErrore apertura socket");
	       	return -1;
	}
	sockbd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	setsockopt(sockbd,SOL_SOCKET,SO_BROADCAST, &broadcastperm, sizeof(broadcastperm));

	memset( &remote_addr, 0, sizeof(remote_addr));		// pulisci memoria
	remote_addr.sin_family = AF_INET;			// famiglia socket (IPv4)
	remote_addr.sin_addr.s_addr = inet_addr(argv[1]); 	// setto indirizzo
	remote_addr.sin_port = htons(atoi(argv[2]));		// setto porta
	
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(33340);
	broadcast_addr.sin_addr.s_addr = INADDR_ANY;
	bind(sockbd, (struct sockaddr*) &broadcast_addr, sizeof(broadcast_addr));
	
	while(fgets_noNewline(sendline,1000,stdin)!=NULL)
	{
		sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr*) &remote_addr,
				sizeof(remote_addr));
		printf("Ho appena mandato [%s]\n", sendline);	
		memset(&sendline,0,strlen(sendline));
		recvfrom(sockbd,recvline,1000,0,(struct sockaddr*) &broadcast_addr, &blen);
		printf("\nFrom [%s:%d]: \"%s\"\n",
				inet_ntoa(broadcast_addr.sin_addr),
				ntohs(broadcast_addr.sin_port),
				recvline);
		memset(&recvline,0,strlen(recvline));
	}
}
