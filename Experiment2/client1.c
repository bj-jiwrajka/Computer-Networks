// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// #define PORT 8080

// int main() {
//     int client_fd;
//     struct sockaddr_in serv_addr;
//     char buffer[1024] = {0};
    
//     // Create socket file descriptor
//     if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }
    
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);
    
//     // Convert IPv4 and IPv6 addresses from text to binary form
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         perror("Invalid address/ Address not supported");
//         exit(EXIT_FAILURE);
//     }
    
//     // Connect to the server
//     if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//         perror("Connection failed");
//         exit(EXIT_FAILURE);
//     }
    
//     while (1) {
//         memset(buffer, 0, sizeof(buffer));
//         // Send data to server (Client 1)
//         printf("Client 1, Enter your message: ");
//         fgets(buffer, sizeof(buffer), stdin);
//         send(client_fd, buffer, strlen(buffer), 0);
        
//         memset(buffer, 0, sizeof(buffer));
//         // Receive data from server (Client 2)
//         recv(client_fd, buffer, sizeof(buffer), 0);
//         printf("Client 2 to Client 1: %s", buffer);
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
    int serv_fd;
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

    if (connect(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("error");
        return 0;
    }

    while (1)
    {
        char message[1024];
        printf("Enter your message to the server: ");
        fgets(message, sizeof(message), stdin);
        send(serv_fd, message, sizeof(message), 0);

        char buffer2[567] = {0};
        recv(serv_fd, buffer2, sizeof(buffer2), 0);

        printf("CLient 2 to client 1: %s", buffer2);
    }
    return 0;
}