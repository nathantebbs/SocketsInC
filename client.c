#include "payload.h"
#include <netdb.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc != 3) {
        error(1, "USAGE: ./client <hostname> <port_number>\n");
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error(1, "ERROR: Unable to open socket\n");

    server = gethostbyname(argv[1]);
    if (server == NULL)
        error(1, "ERROR: No such host\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error(1, "ERROR: Unable to connect socket");

    char *line = NULL;
    size_t bufsize = 0;

    while (1) {
        ssize_t nread = getline(&line, &bufsize, stdin);
        if (nread == -1)
            break;

        Payload p;
        p.length = (uint32_t)nread;
        p.body = line;

        if (send_payload(sockfd, &p) < 0) {
            perror("ERROR: Unable to send payload");
            break;
        }
    }

    free(line);
    close(sockfd);
    return 0;
} // main()
