#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    const char *message = "Hello, from the client";
    sendto(clientSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    socklen_t serverAddrLen = sizeof(serverAddr);
    ssize_t dataSize = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &serverAddrLen);

    gettimeofday(&end_time, NULL);

    double rtt_ms = (double)(end_time.tv_sec - start_time.tv_sec) * 1000.0 + (double)(end_time.tv_usec - start_time.tv_usec) / 1000.0;

    if (dataSize == -1) {
        perror("Error receiving data");
    } else {
        buffer[dataSize] = '\0';
        printf("Received from server: %s\n", buffer);
        printf("Round trip time: %.2f ms\n", rtt_ms);
    }

    close(clientSocket);
    return 0;
}
