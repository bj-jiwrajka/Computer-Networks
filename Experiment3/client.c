// Client
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
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    struct Frame frame;
    frame.seq_num = 0;
    // Prepare data for the frame (if needed)

    for (int i = 0; i < 20; ++i) {
        frame.seq_num = i;
        send(sockfd, &frame, sizeof(frame), 0);
        printf("Sent frame %d\n", frame.seq_num);
    }

    close(sockfd);
    return 0;
}

