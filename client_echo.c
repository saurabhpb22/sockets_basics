#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>

#define ERROR (-1)
#define BUF_SZ (1024)
#define KILL_CMD "killcl"

int main (int argc, char **argv)
{
	/* Checking if required arguments are available */
        if (argc < (1+2))
        {
                printf("Following cmdline args required:\n");
                printf("-Address of the remote server xxx.xxx.xxx.xxx :\n");
		printf("-Port number to listen on : ?\n\n");
                exit(-1);
        }

	/* Variables declaration */
	struct sockaddr_in remoteServerStruct;
	int remoteServerSock;
	char input[BUF_SZ], output[BUF_SZ];
	int len;

	/* Creating remoteServerSocket for remote server */
	if ((remoteServerSock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("remoteServerSocket() error");
		exit(-1);
	}

	printf(">Remote-server-remoteServerSocket created..\n");

	remoteServerStruct.sin_family = AF_INET;
	remoteServerStruct.sin_port = htons(atoi(argv[2]));
	remoteServerStruct.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&remoteServerStruct.sin_zero, 0);

	/* Connect to the remote server using port and address */
	if ((connect(remoteServerSock, (struct sockaddr *)&remoteServerStruct, 
		sizeof(struct sockaddr))) == ERROR)
	{
		perror("connect() error");
		exit(-1);
	}

	printf(">Connection to remote-server[%s,%d] established\n",
		(char*)inet_ntoa(remoteServerStruct.sin_addr),(int)remoteServerStruct.sin_port);
	printf(">Enter '%s' to kill client\n",KILL_CMD);

	printf(">Enter data(1msgLimit:%dB) to send to the remote-server:\n",BUF_SZ);
	while(1)
	{
		fgets(input, BUF_SZ, stdin);

		if (strncmp(input,KILL_CMD,strlen("killcl")-1) == 0)
		{
			break;
		}
		send(remoteServerSock, input, strlen(input),0);
		printf(">[SENT]: %s",input);

		len = recv(remoteServerSock, output, BUF_SZ, 0);
		output[len] = '\0';
		printf(">[RECV]: %s", output);
	}

	printf(">Closing remote-server-remoteServerSocket\n");
	close(remoteServerSock);


return 0;
}
