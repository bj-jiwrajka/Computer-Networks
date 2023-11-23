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
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    char buffer[256];

    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");

    int error = 0;

    while (1) {
        n = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            printf("Connection closed by client.\n");
            break;
        }
        error++;

        // Simulate frame loss for demonstration purposes
        if (error % 3 != 0) {
            printf("Received: %s\n", buffer);
            send(newsockfd, "ACK", 3, 0);
        } else {
            printf("Frame lost. Not sending ACK.\n");
            send(newsockfd, "NOT", 3, 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
