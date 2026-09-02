#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int max_fd;
	// server file descriptor
	int server_fd;
	// create IPv4, TCP style byte stream, use the default protocol for this socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	max_fd = server_fd;
	
	// temp set
	fd_set ready;
	FD_ZERO(&ready);
	// master set
	fd_set master;
	FD_ZERO(&master);
	
	// create an address structure
	struct sockaddr_in server_addr;
	// IPv4
	server_addr.sin_family = AF_INET;
	// port  8080 (can be any)
	// htons converts 8080 into specific byte order
	server_addr.sin_port  = htons(8080);
	// Use any IPv4 network interface (wire,WiFi ..)
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
	if(server_fd == -1)
	{
		perror("socket");
		
		return 1;
	}
	
	printf("socket created: %d\n", server_fd);
	
	// associate socket with the IP address and port
	if(bind(server_fd, (struct sockaddr *)&server_addr,sizeof server_addr) == -1)
	{
		perror("bind");
		
		return 1;
	}
	
	printf("socket bound to port 8080\n");
	
	if(listen(server_fd, 5) == -1)
	{
		perror("listen");
		
		return 1;
	}
	
	printf("server is listening  on port 8080\n");
	
	// Add server socket to the master set
	FD_SET(server_fd, &master);
	
	while(1)
	{
		// copy master
		ready = master;
		// monitor new activity
		int sel  = select(max_fd + 1, &ready, NULL, NULL, NULL);
		printf("select: %d\n", sel);
		
		printf("ready: \n");
		for(int i = 0;  i <= max_fd; i++)
		{
			if(FD_ISSET(i, &ready))
			{
				printf("%d\n", i);
			}
		}
		
		for(int fd = 0; fd <= max_fd; fd++)
		{
			if(FD_ISSET(fd, &ready))
			{
				if(fd == server_fd)
				{
					int client_fd = accept(server_fd, NULL, NULL);
					
					FD_SET(client_fd, &master);
					
					if(client_fd > max_fd)
					{
						max_fd = client_fd;
					}
					
					printf("new connection: %d\n", client_fd);
					printf("total: %d\n", max_fd);
					printf("master: \n");
					for(int i = 0;  i <= max_fd; i++)
					{
						if(FD_ISSET(i, &master))
						{
							printf("%d\n", i);
						}
					}
				}
				else
				{
					char buffer[1024];
					
					int bytes_read = recv(fd, buffer,sizeof(buffer) - 1, 0);
					if(bytes_read == 0)
					{
						close(fd);
						FD_CLR(fd, &master);
					}
					else if(bytes_read > 0)
					{
						buffer[bytes_read] = '\0';
						printf("client %d: %s", fd, buffer);
						
						for(int i = 0; i <= max_fd; i++)
						{
							if(FD_ISSET(i, &master) && i != server_fd && i != fd)
							{
								if(send(i, buffer, bytes_read, 0) == -1)
								{
									perror("send");
								}
							}
						}
					}
				}
			}
		}
	}
	
	close(server_fd);
	
	return 0;
}
