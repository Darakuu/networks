// UDP SenderString
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* fgets_noNewline(char *msg, size_t msglen, FILE *keyinput)
{
	if(fgets(msg,msglen,keyinput) != 0)
	{
		msg[strcspn(msg, "\n")] = '\0';
		return msg;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int sockfd;
	struct sockaddr_in dest_addr;
	extern int errno;
	int i;
	char line[100];
	char msg[256];	

	if (argc<3)
	{
		printf("USA: UDP_sender <IP_dest> <PORT_dest>\n"); 
		return 0;
	}
	
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("\nErrore apertura socket");
	       	return -1;
	}
	memset( &dest_addr, 0, sizeof(dest_addr));	// pulisci memoria
	dest_addr.sin_family = AF_INET;			// famiglia socket (IPv4)
	dest_addr.sin_addr.s_addr = inet_addr(argv[1]); // setto indirizzo
	dest_addr.sin_port = htons(atoi(argv[2]));	// setto porta
	for(;;)
	{
		printf("\nManda un messaggio: ");
		//scanf("%s",&msg);
		fgets_noNewline(msg,sizeof(msg),stdin);
		printf("Sending to %s : %d\n",inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port));
		//printf("%s\n", msg);
		sendto(sockfd, msg, strlen(msg)+1, 0,
			(struct sockaddr *) &dest_addr, sizeof(dest_addr) );
	}
}
