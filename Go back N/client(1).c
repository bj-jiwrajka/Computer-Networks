#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define WINDOW_SIZE 4
#define FRAME_COUNT 10
#define PORT 3003
void sender() {
    int sender_socket;
    struct sockaddr_in server_addr;
    
    sender_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(sender_socket == -1) {
        perror("[-] Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server_addr.sin_port = htons(PORT);
    
    if(connect(sender_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("[-] Connection failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connected to server\n");
    const char* frames[FRAME_COUNT] = {"Frame0 ", "Frame1 ", "Frame2 ", "Frame3 ", "Frame4 ", "Frame5 ", "Frame6 ", "Frame7 ", "Frame8 ", "Frame9 "};
    int base = 0;
    
    while (base < FRAME_COUNT) {

        for (int i = base; i < base + WINDOW_SIZE && i < FRAME_COUNT; i++) {
            send(sender_socket, frames[i], strlen(frames[i]), 0);
            printf("[+] Sent: %s\n", frames[i]);
        }
        
        int ack;
        recv(sender_socket, &ack, sizeof(ack), 0);
        printf("[+] Received ACK: %d\n", ack);

        base = ack + 1;
        sleep(1);
    }
    
    close(sender_socket);
}

int main() {
    sender();
    return 0;
}
