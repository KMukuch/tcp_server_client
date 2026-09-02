# TCP Server in C - Workflow

## Headers

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
```

`stdio.h` -> `printf()`, `getchar()`, etc.

`stdlib.h` -> `exit()`, etc.

`unistd.h` -> `read()`, `write()`, `close()`

`sys/socket.h` -> `socket()`, `bind()`, `listen()`, `accept()`

`netinet/in.h` -> `struct sockaddr_in`

`arpa/inet.h` -> `htons()`, IP/address helpers

## Create a socket

```c
int sockfd;

sockfd = socket(AF_INET, SOCK_STREAM, 0);
```

`AF_INET` -> IPv4

`SOCK_STREAM` -> TCP

`sockfd` -> handle used by your program to refer to the socket managed by the OS

```text
C program -> sockfd -> OS -> TCP/IP -> network
```

## Configure the server address

```c
struct sockaddr_in servaddr;

servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(8080);
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
```

`struct sockaddr_in` -> C structure that holds IPv4 address information.

Conceptually:

```text
IP -> this computer
Port -> 8080
```

A port is a logical number, not a physical port.

## Bind

```c
bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
```

Tell the OS:

```text
"Associate this socket with this local IP and port."
```

## Listen

```c
listen(sockfd, 5);
```

Tell the OS:

```text
"This is a server socket. Wait for incoming TCP connections."
```

## Accept

```c
int connfd;

connfd = accept(sockfd, NULL, NULL);
```

Wait for a client to connect.

The two sockets have different jobs:

```text
sockfd -> listening socket -> waits for new clients

connfd -> connection socket -> talks to this client
```

## Communicate

```c
read(connfd, buffer, sizeof(buffer));
```

Receive bytes from the client.

```c
write(connfd, buffer, length);
```

Send bytes to the client.

After `accept()`, you normally communicate with the client through `connfd`.

The OS handles TCP underneath.

## Close

```c
close(connfd);
```

Finish the connection with the client.

```c
close(sockfd);
```

Stop listening.

## Mental model

```text
socket() -> create TCP socket
bind() -> associate socket with IP/port
listen() -> wait for clients
accept() -> get socket for one client
read()/write() -> communicate
close() -> finish
```

## Concepts

```text
IP address -> Which computer?

Port -> Which network endpoint on that computer?

TCP -> Rules for reliable communication

Socket -> OS-managed networking resource

sockfd -> Handle for the listening socket

connfd -> Handle for a client's connection
```

## Socket vs Port

A port is just a logical number:

```text
8080
```

A socket is an OS-managed networking resource:

```text
Socket -> TCP
Socket -> local IP
Socket -> local port
Socket -> connection state
Socket -> buffers
Socket -> other OS-managed state
```

Your C variable:

```c
int sockfd;
```

does not contain all those fields.

It contains a handle that lets your program refer to the socket managed by the OS.

## Whole picture

```text
C program -> socket() -> OS creates socket
OS socket -> bind() -> local IP/port
OS socket -> listen() -> waits for client
accept() -> connection socket
connection socket -> read()/write() -> communicate with client
close() -> finish
```

The C program does not implement TCP itself.

The OS already implements TCP. The C program uses the socket API to tell the OS what it wants.
