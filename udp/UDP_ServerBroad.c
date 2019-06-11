// UDP Server Broadcast
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

char* fgets_nonl(char *msg, size_t msglen, FILE *keyinput)
{
	if(fgets(msg,msglen,keyinput) != 0)
	{
		msg[ strcspn(msg,"\n") ] = '\0';
		return msg;
	}
	return 0;
}


void copy_info(char*msg)
{
	FILE *fp = fopen("logs.txt","w");
	fprintf(fp,"%s\n",msg);
	fclose(fp);
//	printf("Wrote correctly\n");
}

char *read_info()
{
	char *retmsg = (char*)malloc(1000);
//	printf("apro file.\n");
	FILE *fp = fopen("logs.txt","r");
	fgets_nonl(retmsg,1000,fp);
	fclose(fp);
	remove("logs.txt");
//	printf("Read correctly, returning message:[\"%s\"]\n",retmsg);
	return retmsg;
}

int main(int argc, char* argv[])
{
	int sockfd,sockbd,n, pid;
	struct sockaddr_in local_addr, remote_addr, broadcast_addr;
	socklen_t len = sizeof(remote_addr);
	char msg[1000];
	char *readmsg;
	memset(&readmsg,0,1000);

	int broadcastperm = 1;
	char * broadcastIP = "10.0.0.255";
	unsigned short broadcastPORT = 33340;

	if (argc<2)
	{
		printf("USA: ./UDP_ServerBroad <PORTA>\n");
		return 0;
	}

	if ((sockbd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP) )<0)
	{
		printf("Errore broadcast socket binding\n");
		return -10;
	}

	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		printf("Errore di socket binding\n");
		return -1;
	}

	if (setsockopt(sockbd,SOL_SOCKET,SO_BROADCAST, (void*) &broadcastperm, sizeof(broadcastperm)) < 0)
	{
		printf("errore setsockopt\n");
		return -2;
	}
	memset(&local_addr,0,sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(atoi(argv[1]));
	bind(sockfd, (struct sockaddr *) &local_addr, sizeof(local_addr));
	
	memset(&broadcast_addr,0,sizeof(broadcast_addr));
	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_addr.s_addr = inet_addr(broadcastIP);
	broadcast_addr.sin_port = htons(broadcastPORT);
	
	printf("Listening on [%d]...\n",ntohs(local_addr.sin_port));
	for (;;)
	{
		recvfrom(sockfd, msg,1000,0,(struct sockaddr *) &remote_addr, &len);
		copy_info(msg);		
		printf("From [%s:%d]: \"%s\"\n",inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port),msg);
		memset(&msg,0,strlen(msg));
		pid = fork();
		if (pid==0)
		{
			readmsg = read_info();
			sendto(sockbd,readmsg,strlen(readmsg),0,(struct sockaddr *) &broadcast_addr, sizeof(broadcast_addr));
			exit(0);
		}
	}
}
