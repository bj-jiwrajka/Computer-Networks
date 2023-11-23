#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

// Function to serve an HTML page
void serve_html(int client_socket) {
    const char* html_content = 
        "<html>\n"
        "<head><title>Simple HTTP Server</title></head>\n"
        "<body>\n"
        "<h1>Hello from the Server!</h1>\n"
        "<p>This is a sample HTML page served by the server.</p>\n"
        "</body>\n"
        "</html>";

    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    
    send(client_socket, response, strlen(response), 0);
    send(client_socket, html_content, strlen(html_content), 0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("Error in listening");
        exit(1);
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error in accepting connection");
            continue;
        }

        serve_html(client_socket);

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
