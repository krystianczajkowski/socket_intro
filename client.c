#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define error(msg) \
            do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s port", argv[0]);
        exit(1);
    }
    int sockfd, s, port;
    char buf[500];
    size_t len;
    ssize_t nread;
    struct addrinfo hints;
    struct addrinfo *res, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    s = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }    
    for (rp = res; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, SOCK_STREAM, 0);
        if (sockfd == -1) 
            continue;
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
    }
    freeaddrinfo(res);

    if (rp == NULL) {
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }
    while (1) {
        memset(&buf, 0, sizeof(buf));
        printf("YOU: ");
        fgets(buf, sizeof(buf), stdin);
        s = write(sockfd, buf, sizeof(buf));
        if (s == -1) {
            error("Error writing to server\n");
        }
        // memset(&buf, 0, sizeof(buf));
        // if ((s = read(sockfd, buf, sizeof buf)) == -1)
        //     error("Error writing to server\n");
        // printf("SERVER: %s", buf);
    }

    return 0;
}