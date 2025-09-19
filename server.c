#include "payload.h"
#include <sys/socket.h>

int main(int argc, char *argv[]) {

    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;

    if (argc != 2)
        error(1, "USAGE: ./server <port_number>");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error(1, "ERROR: Unable to open socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error(1, "ERROR: Unable to bind socket");

    if (listen(sockfd, 5) < 0)
        error(1, "ERROR: Unable to listen on socket");

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (newsockfd < 0) {
            perror("ERROR: Unable to accept incoming connection");
            continue;
        }

        printf("--- New connection received --\n");

        while (1) {
            Payload *p = recv_payload(newsockfd);
            if (!p) {
                close(newsockfd);
                break;
            }

            printf("%d\t%s", p->length, p->body);

            free_payload(p);
        }
        printf("--- Connection ended --\n");
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
