// UDP Sender
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int sockfd;
	struct sockaddr_in dest_addr;
	extern int errno;
	int i,n;
	char line[100];
	
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

	printf("\nInserisci un numero intero: ");
	scanf("%d",&n);
	
	printf("Sending to %s : %d\n", inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port));

	for (i=0; i<10; ++i)
	{
		sprintf(line, "%d", i+n);
		printf("%s\n", line);
		sendto(sockfd, line, strlen(line)+1, 0,
				(struct sockaddr *) &dest_addr, sizeof(dest_addr) );
	}
}
