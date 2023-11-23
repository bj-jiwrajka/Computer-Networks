#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3003

void receiver() {
    int receiver_socket, connection_socket;
    struct sockaddr_in receiver_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    receiver_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(receiver_socket == -1) {
        perror("[-] Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = INADDR_ANY;
    receiver_addr.sin_port = htons(PORT); 
    
    if(bind(receiver_socket, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) == -1) {
        perror("[-] Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(receiver_socket, 1);
    printf("[+] Receiver listening on port %d\n", 3003);

    connection_socket = accept(receiver_socket, (struct sockaddr*)&client_addr, &client_addr_len);

    if(connection_socket == -1) {
        perror("[-] Acceptance of client failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Client connected\n");

    int expected_sequence_number = 0;
    char buffer[1024];
    
    while (1) {
        ssize_t bytes_received = recv(connection_socket, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0) {
            printf("[-] Connection closed\n");
            break;
        }
        
        buffer[bytes_received] = '\0';
        
        int sequence_number = buffer[strlen(buffer) - 2] - '0';

        if (sequence_number >= expected_sequence_number) {

            printf("[+] Received: ");
            for(int i = 0; i <= strlen(buffer) - 2; i++) {
                if(buffer[i]==' ') break;
                printf("%c", buffer[i]);
            }
            printf("\n");
            
            send(connection_socket, &expected_sequence_number, sizeof(expected_sequence_number), 0);
            expected_sequence_number = (expected_sequence_number + 1) % 10;
        }
    }
    
    close(connection_socket);
    close(receiver_socket);
}

int main() {
    receiver();
    return 0;
}
