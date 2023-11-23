#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"  // Server IP address
#define SERVER_PORT 8080       // Port on which the server is listening
#define BUFFER_SIZE 1024

// Function to write data to a file
void write_to_file(const char* filename, const char* data) {
    FILE* file = fopen(filename, "w");
    if (file) {
        fputs(data, file);
        fclose(file);
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connecting to the server");
        exit(1);
    }

    char request[] = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(client_socket, request, strlen(request), 0);

    char response[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_socket, response, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error in receiving data");
        exit(1);
    }

    response[bytes_received] = '\0';
    printf("Received response:\n%s\n", response);

    // Parse the HTML content from the response
    char* html_start = strstr(response, "<html");
    if (html_start) {
        write_to_file("downloaded.html", html_start);
        printf("HTML content saved to 'downloaded.html'.\n");
    } else {
        perror("Error parsing HTML content from the response");
    }

    close(client_socket);
    return 0;
}
