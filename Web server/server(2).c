#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

void error(char *msg) {
    perror(msg);
    exit(1);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    FILE *html_file;
    int bytes_read;

    html_file = fopen("index.html", "r");

    if (html_file == NULL) {
        error("Error opening index.html file");
    }

    char http_header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    write(client_socket, http_header, sizeof(http_header) - 1);

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), html_file)) > 0) {
        write(client_socket, buffer, bytes_read);
    }

    fclose(html_file);
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("Error opening socket");
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        error("Error binding socket");
    }

    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        error("Error listening for connections");
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            error("Error accepting connection");
        }
        handle_client(client_socket);
    }

    close(server_socket);

    return 0;
}
