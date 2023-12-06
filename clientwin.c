#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include <ws2tcpip.h>
// #include <winsock.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    // SERVER       CLIENT
    // socket       socket
    // bind         
    // listen       
    // accept       connect
    // read         write
    // write        read
    // close        close
    // --------------------

    int sockfd, port;
    int res = 0;
    struct sockaddr_in sa;
    struct hostent *server;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 6);
    if (sockfd < 0) {
        error("Error opening connection");
    }
    if ((server = gethostbyname(argv[1])) == NULL) {
        error("Error getting host name");
    }
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;

    port = atoi(argv[2]);
    sa.sin_port = htons(port);
    memmove(&sa.sin_addr.s_addr, server->h_addr, server->h_length);
    if (connect(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
        error("Connection failed");
    }

    char buffer[255];
    while (1) {
        memset(&buffer, 0, sizeof(buffer));
        printf("YOU: ");
        fgets(buffer, sizeof(buffer), stdin);
        int n = write(sockfd, buffer, strlen(buffer));
        if (n < 0 ) {
            error("error on write");
        }
        // memset(&buffer, 0, sizeof(buffer));
        // n = read(sockfd, buffer, sizeof(buffer));
        // if (n < 0 ) {
        //     error("error on write");
        // }
        // printf("SERVER: %s", buffer);
        int i = strncmp("bye", buffer, 3);
        if (i == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;
} 