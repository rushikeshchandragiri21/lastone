#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> // Include the string.h header file for strlen

#define BUFFER_SIZE 4096

void parent_process(const char *data, const char *pipe_name) {
    // Open the named pipe for writing
    int fifo_fd = open(pipe_name, O_WRONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write data to the named pipe
    ssize_t bytes_written = write(fifo_fd, data, strlen(data));
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    close(fifo_fd);

    printf("Data sent successfully from parent to child.\n");
}

void child_process(const char *pipe_name) {
    // Open the named pipe for reading
    int fifo_fd = open(pipe_name, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read data from the named pipe
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fifo_fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[bytes_read] = '\0';

    // Close file descriptor
    close(fifo_fd);

    printf("Data received successfully by child: %s\n", buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pipe_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *pipe_name = argv[1];

    // Create the named pipe
    if (mkfifo(pipe_name, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        parent_process("Hello, child!", pipe_name);
    } else if (pid == 0) {
        // Child process
        child_process(pipe_name);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
