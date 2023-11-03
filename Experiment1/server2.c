#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3002

int main() {
    int server_socket, client1_socket, client2_socket, read_size;
    struct sockaddr_in server, client1, client2;
    char client1_message[2000], client2_message[2000];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    listen(server_socket, 3);
    printf("Server listening on port %d\n", PORT);

    // Accept two clients
    int addr_len = sizeof(struct sockaddr_in);
    client1_socket = accept(server_socket, (struct sockaddr*)&client1, (socklen_t*)&addr_len);
    if (client1_socket < 0) {
        perror("Acceptance of client 1 failed");
        exit(EXIT_FAILURE);
    }
    printf("Client 1 connected\n");

    client2_socket = accept(server_socket, (struct sockaddr*)&client2, (socklen_t*)&addr_len);
    if (client2_socket < 0) {
        perror("Acceptance of client 2 failed");
        exit(EXIT_FAILURE);
    }
    printf("Client 2 connected\n");

     while (1) {
        // Clear the buffers
        memset(client1_message, 0, sizeof(client1_message));
        memset(client2_message, 0, sizeof(client2_message));

        // Receive messages from clients
        read_size = recv(client1_socket, client1_message, sizeof(client1_message), 0);
        if (read_size > 0) {
            printf("Client 1: %s", client1_message);
            send(client2_socket, client1_message, strlen(client1_message), 0);
        }

        read_size = recv(client2_socket, client2_message, sizeof(client2_message), 0);
        if (read_size > 0) {
            printf("Client 2: %s", client2_message);
            send(client1_socket, client2_message, strlen(client2_message), 0);
        }
    }
    close(client1_socket);
    close(client2_socket);
    close(server_socket);

    return 0;
}