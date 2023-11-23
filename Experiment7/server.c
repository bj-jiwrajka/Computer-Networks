#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 3080
#define BUFFER_SIZE 1024

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int opt = 1;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error in socket creation");
        exit(1);
    }
    if (setsockopt(server_socket, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error in binding");
        exit(1);
    }
    if (listen(server_socket, 5) < 0)
    {
        perror("Error in listening");
        exit(1);
    }
    printf("Server is listening on port %d...\n", PORT);

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0)
    {
        perror("Error in accepting connection");
        exit(1);
    }
    char command[BUFFER_SIZE];
    int command_len = recv(client_socket, command, sizeof(command), 0);
    if (command_len < 0)
    {
        perror("Error in receiving data");
        exit(1);
    }

    command[command_len] = '\0';
    printf("Received command: %s\n", command);

    FILE *fp;
    char result[BUFFER_SIZE];
    fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Error executing the command");
        exit(1);
    }

    if (fp != NULL)
    {
        size_t newLen = fread(result, 1, BUFFER_SIZE, fp);
        if (ferror(fp) != 0)
        {
            fputs("Error reading file", stderr);
        }
        else
        {
            result[newLen++] = '\0';
        }
        fclose(fp);
    }

    send(client_socket, result, strlen(result), 0);
    close(client_socket);
    close(server_socket);
    return 0;
}