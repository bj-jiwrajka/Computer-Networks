#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SERVER_IP "127.0.0.1"
#define PORT 3080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char command[BUFFER_SIZE], response[BUFFER_SIZE];
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        exit(1);
    }
    printf("Enter a command to execute on the server: ");
    fgets(command, sizeof(command), stdin);
    send(client_socket, command, strlen(command), 0);
    recv(client_socket, response, sizeof(response), 0);
    printf("Result from the server: %s\n", response);
    return 0;
}