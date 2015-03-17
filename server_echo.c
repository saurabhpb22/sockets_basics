#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>

#define ERROR (-1)
#define BUF_SZ (1024)
#define MAX_CLIENTS (2)

int main (int argc, char **argv)
{
	/* Checking if required arguments are available */
	if (argc < (1+1))
	{
		printf("Following cmdline args required:\n");
		printf("-Port number to listen on : ?\n\n");
		exit(-1);

	}

	/* Variables declaration */
	struct sockaddr_in svr_str;
	struct sockaddr_in cli_str;
	int svr_sock, cli_sock;
	char input[BUF_SZ];
	int sockaddr_in_len = sizeof(struct sockaddr_in);

	/* Creating socket for server */
	if ((svr_sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("socket() error!\n");
		exit(-1);
	}
	printf(">Server socket created..\n");

	svr_str.sin_family = AF_INET;
	svr_str.sin_port = htons(atoi(argv[1]));
	svr_str.sin_addr.s_addr = INADDR_ANY;
	bzero(&svr_str.sin_zero, 8);

	/* Binding created socket with configured port and address */
	if ((bind(svr_sock, (struct sockaddr *)&svr_str,
		sockaddr_in_len)) == ERROR)
	{
		perror("bind() error");
		exit(-1);
	}
	printf(">Server socket is now bound to port no %d and address %s\n",
	(int)ntohs(svr_str.sin_port),(char *)inet_ntoa(svr_str.sin_addr));

	/* Listening to clients on the created and bound server socket */
	if (listen(svr_sock,MAX_CLIENTS) == ERROR)
        {
                perror("listen() error");
                exit(-1);
        }
	printf(">Server listening for max %d clients\n", MAX_CLIENTS);

	while(1)
	{
		printf(">Blocking call to accept(),\n\nwaiting for client(s) ~\n");
		if ((cli_sock = accept(svr_sock,(struct sockaddr *)&cli_str, &sockaddr_in_len)) == ERROR)
		{
			perror("accept() error");
			exit(-1);
		}
		printf("\n>New client[%s,%d] connected\n", 
		(char *)inet_ntoa(cli_str.sin_addr),(int)ntohs(cli_str.sin_port));
		int data_len = 1;

		printf(">Waiting for receiving data(1msgLimit=%dB) which will be echo-ed back\n",BUF_SZ);

		while(data_len)
		{
			data_len = recv(cli_sock, input, BUF_SZ, 0);
			if (data_len)
			{
				send(cli_sock,input,data_len,0);
				input[data_len] = '\0';
					printf(">$%s", input);
			}
		}
		printf(">Client[%s,%d] disconnected :(\n",(char*)inet_ntoa(cli_str.sin_addr),(int)cli_str.sin_port);
		close(cli_sock);
	}

	close(svr_sock);


return 0;
}
