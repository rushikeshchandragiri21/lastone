#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 5005

int main() {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind socket to the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP echo server is running...\n");

    char buffer[BUF_SIZE];
    int len, n;

    while (1) {
        // Receive message from client
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, (char *)buffer, BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        printf("Received message from %s:%d: %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);

        // Echo back the message to client
        sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);

    return 0;
}
netstat -tuln | grep :5005
