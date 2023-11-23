#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3080
#define BUFFER_SIZE 4096

void handle_client(int client_socket)
{
    char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
                      "<h1>Hello World!</h1>"
                      "<p>Click <a href='http://localhost:3080'>here</a> to go back to the main page.</p>";
    send(client_socket, response, strlen(response), 0);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t addr_len = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Binding failed");
        exit(1);
    }

    if (listen(server_socket, 5) < 0)
    {
        perror("Listening failed");
        exit(1);
    }

    printf("Server listening on port %d...\n Click on http://localhost://%d", PORT, PORT);

    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_len);
        if (client_socket < 0)
        {
            perror("Accepting connection failed");
            exit(1);
        }

        handle_client(client_socket);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}
