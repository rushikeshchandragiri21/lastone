#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 4096

void parent_process(const char *file_path, int pipe_fd_write) {
    // Open the file to be sent
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from the file and write to the pipe
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(pipe_fd_write, buffer, bytes_read) != bytes_read) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close file descriptor
    close(file_fd);
    close(pipe_fd_write);

    printf("File sent successfully from parent to child.\n");
}

void child_process(int pipe_fd_read, const char *output_file) {
    // Open the output file for writing
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (output_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from the pipe and write to the output file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(pipe_fd_read, buffer, BUFFER_SIZE)) > 0) {
        if (write(output_fd, buffer, bytes_read) != bytes_read) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close file descriptor
    close(output_fd);
    close(pipe_fd_read);

    printf("File received successfully by child.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_path> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];
    const char *output_file = argv[2];

    // Create the unnamed pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        close(pipe_fd[0]); // Close unused read end
        parent_process(file_path, pipe_fd[1]); // Send file to child through write end
    } else if (pid == 0) {
        // Child process
        close(pipe_fd[1]); // Close unused write end
        child_process(pipe_fd[0], output_file); // Receive file from parent through read end
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}

