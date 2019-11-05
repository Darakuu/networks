// A simple chat client, made for practice
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

char * fgets_nonl(char*msg, size_t len, FILE *fp)	// wrapper function for fgets, changes final '\n' to '\0'
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
	int sockfd, sockoc, newsockoc, n, pid, pid2, len;
	struct sockaddr_in local_addr, dest_addr;
	char* sendmsg= (char*) malloc(1000);
	char* recvmsg= (char*) malloc(1000);

	if (argc != 3)
	{
		printf("USA: client_tcp <IP> <PORTA>\n");
		return 1;
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);		// Socket client
	sockoc = socket(AF_INET,SOCK_STREAM,0);		// Socket server

	pid = fork(); // First fork(), 'splits' the program in server and client
	if(pid==0) // ["server"]
	{
		memset((char*) &local_addr,0,sizeof(local_addr));
		local_addr.sin_family=AF_INET;
		local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		local_addr.sin_port = htons(atoi(argv[2]));
		
		bind(sockoc, (struct sockaddr *) &local_addr, sizeof(local_addr));

		listen(sockoc, 5); // listen(socket, waiting_queue_size)
		printf ("\n[%d] Listening on [%d]\n", getpid(), ntohs(local_addr.sin_port));

		for(;;)
		{
			len = sizeof(dest_addr);
			while((newsockoc = accept(sockoc, (struct sockaddr*) &dest_addr, &len))<0)
			{
				printf("Acceptance failed, retrying in 5 seconds...\n");
				sleep(5);
			}
			
			close(sockoc);
			
			for(;;)
			{
				n = recv(newsockoc,recvmsg,999,0);
				if(n==0) return 0;
				recvmsg[n] = 0;
				printf("\nPID[%d]: ricevuto %s:%d => [\"%s\"]\n",
						getpid(), 
						inet_ntoa(dest_addr.sin_addr),
						ntohs(dest_addr.sin_port),recvmsg
					);
			}
			return 0;
		}
		
	}
	else // ["client"]
	{

		memset(&dest_addr,0,sizeof(dest_addr));
		dest_addr.sin_family = AF_INET;		
		dest_addr.sin_addr.s_addr = inet_addr(argv[1]);
		dest_addr.sin_port= htons(atoi(argv[2]));

		while (connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0)
		{
			printf("Error connecting, retrying in 5 seconds...\n");
			sleep(5);	
		}
		printf("Connected to: %s:%d,Ready to send messages.\n",
				inet_ntoa(dest_addr.sin_addr),ntohs(dest_addr.sin_port));
		while(sendmsg!=NULL)
		{
			sendmsg = fgets_nonl(sendmsg,999,stdin);
			send(sockfd,sendmsg,strlen(sendmsg),0);
		}
		return 0;
	}

}
