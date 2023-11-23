// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// #define PORT 8080

// int main() {
//     int server_fd, client1_fd, client2_fd;
//     struct sockaddr_in address;
//     int opt = 1;
//     int addrlen = sizeof(address);
//     char buffer[1024] = {0};
    
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }
    
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))!=0) {
//         perror("Setsockopt failed");
//         exit(EXIT_FAILURE);
//     }
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
    
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         perror("Bind failed");
//         exit(EXIT_FAILURE);
//     }
    
//     if (listen(server_fd, 2) < 0) {
//         perror("Listen failed");
//         exit(EXIT_FAILURE);
//     }
    
//     printf("Server is listening...\n");
    
//     if ((client1_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
//         perror("Accept failed");
//         exit(EXIT_FAILURE);
//     }
    
//     printf("First client connected\n");
    
//     if ((client2_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
//         perror("Accept failed");
//         exit(EXIT_FAILURE);
//     }
    
//     printf("Second client connected\n");
    
//     while (1) {
//         memset(buffer, 0, sizeof(buffer));
        
//         // Receive data from client 1
//         recv(client1_fd, buffer, sizeof(buffer), 0);
//         printf("Client 1: %s", buffer);
        
//         // Send data from client 1 to client 2
//         send(client2_fd, buffer, strlen(buffer), 0);
        
//         memset(buffer, 0, sizeof(buffer));
        
//         // Receive data from client 2
//         recv(client2_fd, buffer, sizeof(buffer), 0);
//         printf("Client 2: %s", buffer);
        
//         // Send data from client 2 to client 1
//         send(client1_fd, buffer, strlen(buffer), 0);
//     }
    
//     return 0;
// }

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#define PORT 8081

int main()
{
    int serv_fd, cli_fd, cli2_fd;
    struct sockaddr_in serv_addr;
    int addrlen = sizeof(serv_addr);
    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("error");
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("error");
        return 0;
    }

    if (listen(serv_fd, 4) < 0)
    {
        perror("error");
        return 0;
    }
    if ((cli_fd = accept(serv_fd, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen)) < 0)
    {
        perror("error");
        return 0;
    }

    if ((cli2_fd = accept(serv_fd, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen)) < 0)
    {
        perror("error");
        return 0;
    }

    while (1)
    {
        char buffer[567] = {0};
        recv(cli_fd, buffer, sizeof(buffer), 0);
        printf("Client  to client 2: %s", buffer);
        send(cli2_fd, buffer, sizeof(buffer), 0);
        // printf("Client 2 to client 2: %s", buffer);

        char buffer2[567] = {0};
        recv(cli2_fd, buffer2, sizeof(buffer2), 0);
        printf("Client 2 to client 1: %s", buffer2);
        send(cli_fd, buffer2, sizeof(buffer2), 0);
    }
}