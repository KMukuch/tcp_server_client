# TCP broadcast server (multiple clients)

A multi-client TCP server that receives messages from one client and broadcasts them to the other connected clients.

There are several versions available on the correspoding branch.

## TCP broadcast server using `select()`

Uses `select()` to monitor the server socket and multiple client sockets from a single event loop.

## TCP broadcast server using `poll()`

Uses `poll()` to monitor the server socket and multiple client sockets using an array of `struct pollfd`.

## TCP broadcast server using `epoll()`



## TCP broadcast server using threads