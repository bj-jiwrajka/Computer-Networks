#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define port 5001
int main()
{
    int sock_fd;
    struct sockaddr_in serv_addr;

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("error");
        return 0;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error");
        return 0;
    }
    char str[678] = {};
    char *buffer = "wbfjlgn jonoe\n";
    send(sock_fd, buffer, strlen(buffer), 0);
    recv(sock_fd, str, sizeof(str), 0);
    printf("%s\n", str);
    return 0;
}