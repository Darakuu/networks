// To be used along with TCP_Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char**argv)
{
	int sockfd, newsockfd,n,pid;
	struct sockaddr_in local_addr, remote_addr;
	socklen_t len;
	char msg[1000];

	if (argc<2)
	{
		printf("Use: server_tcp <PORT>\n");
		return 0;
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if (sockfd < 0)
	{
		printf("Error while attempting to open socket\n");
		return -1;
	}

	memset((char*) &local_addr,0,sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(atoi(argv[1]));

	if(bind(sockfd,(struct sockaddr *) &local_addr, sizeof(local_addr))<0)
	{
		printf("Socket binding error: %d\n",errno);
		return -1;
	}

	listen(sockfd,5);
	printf("Listening on %d...\n", ntohs(local_addr.sin_port));
	for(;;)
	{
		len = sizeof(remote_addr);
		newsockfd = accept(sockfd, (struct sockadddr *) &remote_addr, &len);
		pid = fork();
		if (pid == 0) // child
		{
			close(sockfd);
			for(;;)
			{
				n = recv(newsockfd,msg,999,0);
				if (n == 0) return 0;
				msg[n] = 0;
				printf("PID[%d]: Received by %s:%d => [\"%s\"]\n",
						getpid(),inet_ntoa(remote_addr.sin_addr),
						ntohs(remote_addr.sin_port),msg);
				send(newsockfd,msg,n,0);
			}
			return 0;
		}
		else
			close(newsockfd);
	}
}
