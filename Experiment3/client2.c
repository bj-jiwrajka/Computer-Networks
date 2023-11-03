#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

struct Frame
{
    int seq_num;
    char data[1024];
};

struct Ack
{
    int ack_num;
};

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    struct Frame frame;
    struct Ack ack;
    frame.seq_num = 0;

    for (int i = 0; i < 10; ++i)
    {
        frame.seq_num = i;
        send(sockfd, &frame, sizeof(frame), 0);
        printf("Sent frame %d\n", frame.seq_num);
        sleep(1);
        ssize_t bytes_received = recv(sockfd, &ack, sizeof(ack), 0);
        if (bytes_received <= 0)
        {
            perror("Receive acknowledgment error");
            break;
        }
        printf("Received acknowledgment for frame %d\n", ack.ack_num);
    }

    close(sockfd);
    return 0;
}
