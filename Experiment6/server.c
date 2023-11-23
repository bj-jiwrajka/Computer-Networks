#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3000
#define FILENAME "file.png"

int main()
{
    int sockfd, cli_fd;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("[-] Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-] Error in binding");
        exit(1);
    }
    printf("[+] Server is listening on port %d...\n", PORT);

    listen(sockfd, 10);
    addr_size = sizeof(new_addr);
    cli_fd = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL)
    {
        perror("[-] Error opening file");
        exit(1);
    }

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        send(cli_fd, buffer, n, 0);
    }

    fclose(fp);
    printf("[+] File data sent successfully.\n");

    return 0;
}
