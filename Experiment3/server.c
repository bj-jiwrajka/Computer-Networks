// Server
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

struct Frame {
    int seq_num;
    char data[1024]; // Modify the size as needed
};

int main() {
    int sockfd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 3) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    new_socket = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
    if (new_socket == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    struct Frame received_frame;
    while (recv(new_socket, &received_frame, sizeof(received_frame), 0) > 0) {
        printf("Received frame %d\n", received_frame.seq_num);
    }

    close(new_socket);
    close(sockfd);
    return 0;
}

