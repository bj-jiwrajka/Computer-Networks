#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int serSocck;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen;
    char buffer[1024];

    serSocck = socket(AF_INET, SOCK_DGRAM, 0);
    if (serSocck == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5000);

    if (bind(serSocck, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Echo server is running...\n");

    while (1) {
        clientAddrLen = sizeof(clientAddr);
        ssize_t dataSize = recvfrom(serSocck, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if (dataSize == -1) {
            perror("Error receiving data");
        } else {
            sendto(serSocck, buffer, dataSize, 0, (struct sockaddr *)&clientAddr, clientAddrLen);
        }
    }

    close(serSocck);
    return 0;
}
