#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 5005
#define BACKLOG 5

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind socket to the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, BACKLOG) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP echo server is running...\n");

    char buffer[BUF_SIZE];
    int connfd;
    struct sockaddr_in cliaddr;
    socklen_t len;

    while (1) {
        // Accept a connection
        connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
        if (connfd < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        while (1) {
            // Receive data from client
            int n = read(connfd, buffer, BUF_SIZE);
            if (n < 0) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }
            if (n == 0) {
                // Connection closed by client
                printf("Connection closed by client\n");
                break;
            }

            // Echo back the received data to the client
            if (write(connfd, buffer, n) < 0) {
                perror("write failed");
                exit(EXIT_FAILURE);
            }
        }

        // Close connection
        close(connfd);
    }

    close(sockfd);

    return 0;
}
//netstat -tuln | grep :5005
