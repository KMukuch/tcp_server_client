# TCP server and client in C using POSIX Socket API

This is a collection of simple server and client programs written in C only for educational purposes.

## TCP send and receive client

A basic TCP client that connects to a server and sends and receivs messages.

```
tcp_send_client/
```

## TCP echo server

A basic TCP server that accepts a client connection and sends received data back to the same client.

```
tcp_echo_server/
```

## TCP broadcast server (multiple clients)

A multi-client TCP server that receives messages from one client and broadcasts them to the other connected clients.

```
tcp_broadcast_multiple_clients_server/
```