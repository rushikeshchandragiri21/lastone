#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

#define FIFO_FILE "myfifo"

int main() {
    int fd;
    char *fifo = FIFO_FILE;

    // Open the FIFO for reading
    fd = open(fifo, O_RDONLY | O_NONBLOCK);

    if (fd == -1) {
        perror("open for reading");
        exit(EXIT_FAILURE);
    }

    // Wait until there is data available in the FIFO
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    struct timeval timeout;
    timeout.tv_sec = 5;  // Timeout after 5 seconds
    timeout.tv_usec = 0;
    int ready = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (ready == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (ready == 0) {
        printf("Timeout: No data available in FIFO.\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Read data from the FIFO
    char buffer[BUFSIZ];
    ssize_t num_bytes = read(fd, buffer, BUFSIZ);

    if (num_bytes == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    buffer[num_bytes] = '\0'; // Add null terminator to make it a string

    printf("Received message: %s\n", buffer);

    // Close the read end of the FIFO
    close(fd);

    return 0;
}
