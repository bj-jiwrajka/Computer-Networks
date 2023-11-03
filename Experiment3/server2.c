#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

struct Frame {
    int seq_num;
    char data[1024]; 
};

struct Ack {
    int ack_num;
};

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
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

    if (listen(sockfd, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    new_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);
    if (new_sock == -1) {
        perror("Acceptance failed");
        exit(EXIT_FAILURE);
    }

    struct Frame received_frame;
    struct Ack ack;

    while (1) {
        ssize_t bytes_received = recv(new_sock, &received_frame, sizeof(received_frame), 0);
        if (bytes_received <= 0) {
            perror("Receive error");
            break;
        }

        printf("Received frame %d\n", received_frame.seq_num);

        // Send an acknowledgment back to the client
        ack.ack_num = received_frame.seq_num;
        send(new_sock, &ack, sizeof(ack), 0);
        printf("Sent acknowledgment for frame %d\n", ack.ack_num);
    }

    close(new_sock);
    close(sockfd);
    return 0;
}
