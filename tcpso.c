#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 5005

int main() {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP socket connected to the server...\n");

    char buffer[BUF_SIZE];
    printf("Enter message to send to server: ");
    fgets(buffer, BUF_SIZE, stdin);

    // Send data to server
    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from server
    n = read(sockfd, buffer, BUF_SIZE);
    if (n < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    printf("Received message from server: %s\n", buffer);

    // Close socket
    close(sockfd);

    return 0;
}
