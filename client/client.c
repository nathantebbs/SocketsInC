/*
** File: client.c
** Author: Nathan Tebbs
** Purpose: Simple socket client implementation. I have included extra
**          block comments for personal use when trying to understand this
**          code later on.
**
** TODO: Add multiple msg support:
**           - Once you send a msg and the client confirms the client
**           program closes. We need a loop for input.
**
** Works Cited:
**     - https://www.linuxhowtos.org/C_C++/socket.htm
*/

#include <netdb.h>
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
** Purpose:    print defined error msg, exit with provided exitCode.
** Pre-cond.:  msg is not null, valid exit code.
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
    ** n:         Number of characters read or writen (From read()/write())
    */
    int sockfd, portno, n;

    // sockaddr_in: Structure containing an internet address.
    struct sockaddr_in serv_addr;
    // hostent: Structure containing host information
    struct hostent *server;

    char buffer[256]; // Input buffer

    if (argc != 3) {
        error(0,
              "ERROR: Invalid Arguments\nUSAGE: ./client <SERVER_IP> <PNUM>\n");
    }

    // Assign port number & create the socket
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        error(0, "ERROR: No such host");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr)); // Zero out buffer
    serv_addr.sin_family = AF_INET;               // Code for address family
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);            // Copy host address to serv_addr
    serv_addr.sin_port = htons(portno); // port number. NOTE: htons() converts
                                        // input to network byte order

    // Establish connection with server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error(1, "ERROR: Unable to connect");
    }

    // Simple msg interaction
    printf("Please enter the msg: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        error(1, "ERROR: Unable to write to socket");
    }
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        error(1, "ERROR: Unable to read from socket");
    }
    printf("%s", buffer);

    return 0;
}
