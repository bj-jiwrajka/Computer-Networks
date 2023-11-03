#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        int num, result;
        int x;
        printf("1.Send a number to calculate factorial\n");
        printf("2 Exit the program\n");
        scanf("%d", &x);
        if (x == 2)
        {
            break;
        }
        printf("Enter number to be sent: ");
        scanf("%d", &num);

        sendto(sockfd, &num, sizeof(num), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        printf("Number sent: %d\n", num);

        recvfrom(sockfd, &result, sizeof(result), 0, NULL, NULL);
        printf("Answer received (Factorial of the given number): %d\n\n", result);
    }

    close(sockfd);
    return 0;
}