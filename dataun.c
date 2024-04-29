#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096

void parent_process(const char *data, int pipe_fd_write) {
    // Write data to the pipe
    ssize_t bytes_written = write(pipe_fd_write, data, strlen(data));
    if (bytes_written == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    // Close write end of the pipe
    close(pipe_fd_write);

    printf("Data sent successfully from parent to child.\n");
}

void child_process(int pipe_fd_read) {
    // Read data from the pipe
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(pipe_fd_read, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[bytes_read] = '\0';

    // Close read end of the pipe
    close(pipe_fd_read);

    printf("Data received successfully by child: %s\n", buffer);
}

int main() {
    // Create the unnamed pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        close(pipe_fd[0]); // Close read end of the pipe
        parent_process("Hello, child!", pipe_fd[1]); // Send data to child through write end of the pipe
    } else if (pid == 0) {
        // Child process
        close(pipe_fd[1]); // Close write end of the pipe
        child_process(pipe_fd[0]); // Receive data from parent through read end of the pipe
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
