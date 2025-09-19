#include "payload.h"

/*
** Method error(int exitCode, const char *msg)
** Purpose: Print msg to stderr, exit with exitCode
** Pre-condition: Valid msg
** Post-condition: msg is printed to stderr, Program exited with exitCode
** Parameters:
**   msg -- String that is to be printed
**   exitCode -- Integer code that is to be exited with
** Returns: None.
*/
void error(int exitCode, const char *msg) {
    perror(msg);
    exit(exitCode);
} // error()

/*
** Method write_all(int sockfd, const void *buf, size_t len)
** Purpose: Write len bytes to socket stream
** Pre-condition: Valid socket file descriptor, buffer and length arguments
** Post-condition: len bytes has been written to the socket stream
** Parameters:
**   sockfd -- Socket file descriptor
**   buf -- pointer to the contents that should be written
**   len -- length of the contents in bytes that are to be written
** Returns: (ssize_t) total bytes written
*/
static ssize_t write_all(int sockfd, const void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = write(sockfd, (const char *)buf + total, len - total);
        if (n <= 0)
            return n;
        total += n;
    }
    return total;
} // write_all()

/*
** Method read_all(int sockfd, void *buf, size_t len)
** Purpose: Read len bytes from socket stream
** Pre-condition: Valid socket file descriptor, buffer and length arguments
** Post-condition: len bytes has been read from the socket stream
** Parameters:
**   sockfd -- Socket file descriptor
**   buf -- pointer to the where the content read from the socket stream should
**          be filled
**   len -- length of the contents in bytes that are to be read
** Returns: (ssize_t) total bytes read
*/
static ssize_t read_all(int sockfd, void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = read(sockfd, (char *)buf + total, len - total);
        if (n <= 0)
            return n;
        total += n;
    }
    return total;
} // read_all()

/*
** Method send_payload(int sockfd, const char *msg)
** Purpose: Send the payload contents on the socket stream in the form of a TCP
**          message format being the first 4 bytes describing the length of
**          the msg being sent.
** Pre-condition: Valid socket file descriptor, and payload arguments
** Post-condition: Msg payload has been sent on the socket
**                 stream.
** Parameters:
**   sockfd -- Socket file descriptor
**   p -- Payload structure containing the msg and it's length
** Returns:
**   0 -- Sucess
**   -1 -- Failure
*/
int send_payload(int sockfd, Payload *p) {
    uint32_t net_len = htonl(p->length);

    if (write_all(sockfd, &net_len, sizeof(net_len)) <= 0)
        return -1;
    if (write_all(sockfd, p->body, p->length) <= 0)
        return -1;

    return 0;
} // send_payload()

/*
** Method recv_payload(int sockfd)
** Purpose: Receive the payload being sent on the socket stream and
**          return it to the user.
** Pre-condition: Valid socket file descriptor.
** Post-codnition: Payload recieved and given back to the user as a Payload
**                 object.
** Parameters:
**   sockfd -- Socket file descriptor
** Returns: Payload object
*/
Payload *recv_payload(int sockfd) {
    uint32_t net_len;
    if (read_all(sockfd, &net_len, sizeof(net_len)) <= 0)
        return NULL;

    uint32_t len = ntohl(net_len);
    char *buf = (char *)malloc(len + 1);
    if (!buf)
        return NULL;

    if (read_all(sockfd, buf, len) <= 0) {
        free(buf);
        return NULL;
    }

    buf[len] = '\0';

    Payload *p = (Payload *)malloc(sizeof(Payload));
    if (!p) {
        free(buf);
        return NULL;
    }

    p->length = len;
    p->body = buf;

    return p;
} // recv_payload()

/*
** Method free_payload(Payload *p)
** Purpose: Free the contents memory of a given payload object
** Pre-condition: Valid Payload object
** Post-condition: All memory relating to the given Payload object has
**                 has been freed.
** Parameters:
**   p -- Payload object to be freed
** Returns: None.
*/
void free_payload(Payload *p) {
    if (p) {
        free(p->body);
        free(p);
    }
} // free_payload()
