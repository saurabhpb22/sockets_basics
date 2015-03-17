#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>

#define ERROR (-1)
#define BUF_SZ (1024)

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
	struct sockaddr_in remote_server;
	int sock;
	char input[BUF_SZ], output[BUF_SZ];
	int len;

	/* Creating socket for remote server */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("socket");
		exit(-1);
	}

	printf(">Remote-server-socket created..\n");

	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(atoi(argv[2]));
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&remote_server.sin_zero, 0);

	/* Connect to the remote server using port and address */
	if ((connect(sock, (struct sockaddr *)&remote_server, 
		sizeof(struct sockaddr))) == ERROR)
	{
		perror("connect");
		exit(-1);
	}

	printf(">Connection to remote-server[%s,%d] established\n",
	(char*)inet_ntoa(remote_server.sin_addr),(int)remote_server.sin_port);

	printf(">Enter data(1msgLimit:%dB) to send to the remote-server:\n",BUF_SZ);
	while(1)
	{
		fgets(input, BUF_SZ, stdin);

		#define KILL_CMD "killcl"
		if (strncmp(input,KILL_CMD,strlen("killcl")-1) == 0)
		{
			break;
		}
		send(sock, input, strlen(input),0);
		printf(">[SENT]: %s",input);

		len = recv(sock, output, BUF_SZ, 0);
		output[len] = '\0';
		printf(">[RECV]: %s", output);
	}

	printf(">Closing remote-server-socket\n");
	close(sock);


return 0;
}
