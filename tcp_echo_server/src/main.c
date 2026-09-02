/*
 * main.c
 * 
 * Copyright 2026 mika <mika@debian>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

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
	int bytes_read = 1;
	
	// server file descriptor
	int server_fd;
	// create IPv4, TCP style byte stream, use the default protocol for this socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	// client file descriptor
	int client_fd;
	
	// buffer
	char buffer[1024];

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
	
	client_fd = accept(server_fd, NULL, NULL);
	if(client_fd == -1)
	{
		perror("client");
		
		return 1;
	}
	
	printf("client connected\n");
	
	while(bytes_read > 0)
	{
		// read bytes from client_fd
		bytes_read = read(client_fd, buffer,sizeof(buffer));
		if(bytes_read == -1)
		{
			perror("read");
			
			return 1;
		}
		
		// write bytes in client_fd
		if(write(client_fd, buffer, bytes_read) == -1)
		{
			perror("write");
			
			return 1;
		}
	}
	
	close(client_fd);
	close(server_fd);
	
	return 0;
}

