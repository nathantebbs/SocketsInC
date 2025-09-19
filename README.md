# Sockets in C

> [!WARNING]
> This project is for learning purposes. Feedback is welcome!

The goal is to extend off of the basic socket implementation to provide
support for sending a custom TCP packet to the server.

The architecture of the packet:

- Header: (4 bytes) The length of the payload body
- Body: Content

## Usage

### Client

```sh
./client <SERVER_IP> <PORT_NUMBER>
```

### Server

```sh
./server <PORT_NUMBER
```
