#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_FILE "myfifo"

int main() {
    int fd;
    char *fifo = FIFO_FILE;

    // Open the FIFO for writing
    fd = open(fifo, O_WRONLY);

    if (fd == -1) {
        perror("open for writing");
        exit(EXIT_FAILURE);
    }

    // Write data to the FIFO
    char buffer[BUFSIZ];
    printf("Enter message to send: ");
    fgets(buffer, BUFSIZ, stdin);

    // Remove the newline character from the input
    buffer[strcspn(buffer, "\n")] = '\0';

    ssize_t num_bytes = write(fd, buffer, strlen(buffer));

    if (num_bytes == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    printf("Message sent: %s\n", buffer);

    // Close the write end of the FIFO
    close(fd);

    return 0;
}
