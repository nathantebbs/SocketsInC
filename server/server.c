/*
** File:    server.c
** Author:  Nathan Tebbs
** Purpose: Basic server code for C socket API. I have included some extra
**          block comments for personal use when trying to understand this
**          code later on.
**
** TODO: Add multiple client support:
**           - Multiple clients can theoretically connect but once one
**           sends a msg the server stops running. We need a loop.
**
** Works Cited:
**     - https://www.linuxhowtos.org/C_C++/socket.htm
*/

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
** Method error(char *msg)
** Purpose:    print defined error msg, exit with exitCode.
** Pre-cond.:  msg is not null. Program exits with exitCode
** Post-cond.: msg is printed to stderr, program exits wiht exitCode.
** Parameters:
**     msg - The string that will be printed to stderr
**     exitCode - Usually 0, or 1.
** Returns:    None.
*/

void error(int exitCode, char *msg) {
    perror(msg);
    exit(exitCode);
} // error()

int main(int argc, char *argv[]) {
    /*
    ** sockfd(s): file descriptors, array subscripts into file descriptor table.
    **            Store the values returned by the socket sys call and the
    **            accept sys call.
    **
    ** portno:    Stores the port number which the server accepts connections
    **
    ** clilen:    Stores the size of the address of the client. Needed for
    **            accept sys call.
    **
    ** n:         Number of characters read or writen (From read()/write())
    */
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;

    // sockaddr_in: Structure containing an internet address.
    struct sockaddr_in serv_addr, cli_addr;

    char buffer[256]; // Input buffer

    if (argc != 2) {
        error(1, "ERROR: Invalid Argument\nUSAGE: ./server <PNUM>\n");
    }

    // Create new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error(1, "ERROR: Unable to open socket\n");
    }

    bzero((char *)&serv_addr,
          sizeof(serv_addr)); // Set all values in a buffer to zero.
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;     // Code for address family
    serv_addr.sin_port = htons(portno); // port number. NOTE: htons() converts
                                        // input to network byte order
    serv_addr.sin_addr.s_addr = INADDR_ANY; // IP Address of host

    /*
    ** Binding socket to address w/ socket file descriptor, the address that is
    ** bound, and the size of bound address. NOTE: This will fail if the socket
    ** is already in use on the machine.
    */
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error(1, "ERROR: Unable to bind\n");
    }

    /*
    ** Allows for process to listen on the socket for incoming connections.
    ** Second argument is the size of the backlog queue.
    */
    listen(sockfd, 5);

    /*
    ** accept() - Blocks process until a client connects to the server.
    **            When client connects the method returns a new file
    **            descriptor.
    */
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        error(1, "ERROR: Unable to accept\n");
    }

    bzero(buffer, 256);               // Clear input buffer
    n = read(newsockfd, buffer, 255); // Read from client
    if (n < 0) {
        error(1, "ERROR: Unable to read from socket\n");
    }
    printf("MSG: %s", buffer); // Print back msg sent from client

    // Write confirmation back to client
    char *msg = "SERVER: Msg confirmation\n";
    n = write(newsockfd, msg, strlen(msg));
    if (n < 0) {
        error(1, "ERROR: Unable to write to client\n");
    }

    return 0;
} // main()
