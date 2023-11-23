// =================== Imports =================
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#define PORT 8081

int main() {
    int sock_fd, client_fd;
    struct sockaddr_in serv_addr;
    int address_len = sizeof(serv_addr);

    // Create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return 0;
    }

    // Configuration
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind
    if (bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind socket creation failed");
        return 0;
    }

    // Listen
    if (listen(sock_fd, 3)<0) {
        perror("Listening failed");
        return 0;
    }

    // Accept
    if ((client_fd = accept(sock_fd, (struct sockaddr*)&serv_addr, (socklen_t*)&address_len)) < 0) {
        perror("Accept socket creation failed");
        return 0;
    }

    return 0;
}