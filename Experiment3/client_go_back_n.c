#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd, n;
    socklen_t len;
    char buffer[256];

    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    char frames[WINDOW_SIZE][256] = {"Frame1", "Frame2", "Frame3", "Frame4"};
    int base = 0;
    int nextSeqNum = 0;

    while (base < WINDOW_SIZE) {
        if (nextSeqNum < WINDOW_SIZE) {
            printf("Sending: %s\n", frames[nextSeqNum]);
            send(sockfd, frames[nextSeqNum], strlen(frames[nextSeqNum]), 0);
            nextSeqNum++;
        }

        len = sizeof(serv_addr);
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (buffer == "ACK") {
            printf("Received ACK\n");
            base++;
        } else {
            printf("Timeout! Resending Window\n");
            nextSeqNum = base;
        }
    }

    close(sockfd);
    return 0;
}
