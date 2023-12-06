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
    if (argc != 2) {
        fprintf(stderr, "usage: %s port", argv[0]);
        exit(1);
    }
    int sockfd, newfd, i, port;
    struct sockaddr_in sa, ca;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        error("Socket call failed");
    }
    memset(&sa, 0, sizeof(sa));
    port = atoi(argv[1]);

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET, "172.168.0.7", &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &sa.sin_addr.s_addr, ip_str, sizeof(ip_str)) != NULL) 
        printf("%s\n", ip_str);
    else { 
        error("inet_ntop");
    }

    if ((bind(sockfd, (struct sockaddr *)&sa, sizeof(sa))) == -1) {
        error("Error on bind");
    }
    if ((listen(sockfd, 5)) == -1) {
        error("Error on listen");
    }

    socklen_t ca_len;
    if ((newfd = accept(sockfd, (struct sockaddr *)&ca, &ca_len)) == -1) {
        error("Error on accept");
    }

    if (inet_ntop(AF_INET, &ca.sin_addr.s_addr, ip_str, sizeof(ip_str)) != NULL) 
        printf("%s\n", ip_str);
    else { 
        error("inet_ntop");
    }

    char buffer[4096];
    while (1) {
        memset(&buffer, 0, sizeof(buffer));
        if ((read(newfd, buffer, sizeof(buffer))) == -1) {
            error("Error on read");
        } 
        printf("%s", buffer);
        // memset(&buffer, 0, sizeof(buffer));
        // fgets(buffer, sizeof(buffer), stdin);
        // if ((write(newfd, buffer, sizeof(buffer))) == -1) {
        //     error("Error on write");
        // }
        if ((i = strncmp("bye", buffer, 3)) == 0) {
            break;
        }
    }
    
    close(sockfd);
    close(newfd);
    
    return 0;



}
