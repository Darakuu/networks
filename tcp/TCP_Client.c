#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

char * fgets_nonl(char*msg, size_t len, FILE *fp)
{
	if(fgets(msg,len,fp) != 0)
	{
		msg[strcspn(msg,"\n")] = '\0';
		return msg;
	}
	return NULL;
}

int main(int argc, char*argv[])
{
	int sockfd,n, pid;
	struct sockaddr_in local_addr, dest_addr;
	char* sendmsg= (char*) malloc(1000);
	char* recvmsg= (char*) malloc(1000);

	if (argc != 3)
	{
		printf("USA: client_tcp <IP> <PORTA>\n");
		return 1;
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	memset(&dest_addr,0,sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;		
	dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
	dest_addr.sin_port= htons(atoi(argv[2]));

	connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr));

	while(sendmsg!=NULL)
	{
		printf("Scrivi il messaggio: ");
		sendmsg = fgets_nonl(sendmsg,999,stdin);
		send(sockfd,sendmsg,strlen(sendmsg),0);
		n = recv(sockfd,recvmsg,999,0);
		recvmsg[n] = 0;
		printf("PID=[%d]: Ricevuto da %s:%d => [\"%s\"]\n",
			getpid(),inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port),recvmsg);
	}

}
