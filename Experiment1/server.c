#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<string.h>
#define PORT 5001
int main()
{
    struct sockaddr_in serv_addr;
    int sock_fd, cli_fd;
    int add = sizeof(serv_addr);
    // create
    // configure
    // bind
    // listen
    // accept

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("error");
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr*) & serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error");
        return 0;
    }

    if (listen(sock_fd, 3) < 0)
    {
        perror("error");
        return 0;
    }
    if ((cli_fd = accept(sock_fd, (struct sockaddr*) & serv_addr, (socklen_t*)&add)) < 0)
    {
        perror("error");
        return 0;
    }
    char buffer[3000]={};
    recv(cli_fd, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    char *str = "heom";
    send(cli_fd, str, strlen(str), 0);
}
