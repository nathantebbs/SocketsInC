#ifndef PAYLOAD_H_
#define PAYLOAD_H_

#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Payload {
    uint32_t length;
    char *body;
} Payload;

void error(int exitCode, const char *msg);
int send_payload(int sockfd, Payload *p);
Payload *recv_payload(int sockfd);
void free_payload(Payload *p);

#endif // PAYLOAD_H_
