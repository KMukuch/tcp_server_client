#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

#define MAX_CLIENTS 1024

int main(int argc, char **argv)
{
	// server file descriptor
	int server_fd;
	// create IPv4, TCP style byte stream, use the default protocol for this socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	int fd_counter = 0;
	
	struct pollfd fd_array[MAX_CLIENTS];
	
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
	
	// using an array of pollfd
	init_fd_array(&fd_array, MAX_CLIENTS);
	fd_array[0].fd = server_fd;
	fd_array[0].events = POLLIN;
	fd_counter++;
	
	while(1)
	{
		// detect an activity
		int ready = poll(fd_array, fd_counter, -1);
		if(ready == -1)
		{
			perror("poll");
			
			return 1;
		}
		
		for(int i = 0; i < fd_counter; i++)
		{
			if(fd_array[i].revents & POLLIN)
			{
				if(fd_array[i] == server_fd)
				{
					int client_fd = accept(server_fd, NULL, NULL);
					if(client_fd == -1)
					{
						perror("accept");
					}
					else if(fd_counter < MAX_CLIENTS)
					{
						fd_array[fd_counter].fd = client_fd;
						fd_array[fd_counter].events = POLLIN;
						fd_array[fd_counter].revents = 0;
						fd_counter++;
					}
					
					printf("new client: %d\n", client_fd);
				}
				else
				{
					
				}
			}
		}
	}
	
	close(server_fd);
	
	return 0;
}

void init_fd_array(struct pollfd* fd_array, int fd_length)
{
	if(fd_length < 0)
	{
		perror("fd_length");
		
		return;
	}
	
	for(int i = 0; i < fd_length; i ++)
	{
		fd_array[i].fd = -1;
		fd_array[i].events = POLLIN;
		fd_array[i].revents = 0;
	}
}
