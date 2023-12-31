#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3000

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-] Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Error in connection");
        exit(1);
    }

    printf("[+] Connected to Server.\n");

    printf("[+] Loading content of the file... \n\n");
    FILE *fp = fopen("file2.png", "wb");
    while (1) {
        // memset(buffer, 0, sizeof(buffer));
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            break;
        }
        fprintf(fp, "%.*s", n, buffer);
    }

    printf("\n\n[+] File data received successfully.\n");

    close(sockfd);

    return 0;
}
