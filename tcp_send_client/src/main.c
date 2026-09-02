#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char buffer[1024];

	int socket_fd;
	// create IPv4, TCP style byte stream, use the default protocol for this socket
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);		
	if(socket_fd == -1)
	{
		perror("socket");
		
		return 1;
	}
	
    char *ip_addr = argv[1];
    int port = atoi(argv[2]);
	
    // create an address structure
	struct sockaddr_in server_addr;
	// IPv4
	server_addr.sin_family = AF_INET;
	// port  8080 (can be any)
	// htons converts 8080 into specific byte order
	server_addr.sin_port  = htons(port);
	inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);
	
	printf("socket created: %d\n", socket_fd);
	
	// associate socket with the IP address and port
	if(connect(socket_fd, (struct sockaddr *)&server_addr,sizeof server_addr) == -1)
	{
		perror("connect");
		
		return 1;
	}
	
	printf("connected\n");
	
	while(1)
	{
		fgets(buffer, sizeof(buffer), stdin);
		printf("sending: %s", buffer);

		if(send(socket_fd, buffer, strlen(buffer), 0) == -1)
		{
			perror("send");
			return 1;
		}

		// int n = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
		// if (n == -1)
		// {
		// 	perror("recv");

		// 	return 1;
		// }

		// buffer[n] = '\0';

		// printf("received: %s", buffer);
	}
	
	close(socket_fd);

    return 0;
}