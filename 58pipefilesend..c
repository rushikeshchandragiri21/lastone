#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

void parent_process(const char *file_path, const char *pipe_name) {
    // Open the file to be sent
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the named pipe for writing
    int fifo_fd = open(pipe_name, O_WRONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from the file and write to the named pipe
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(fifo_fd, buffer, bytes_read) != bytes_read) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close file descriptors
    close(fifo_fd);
    close(file_fd);

    printf("File sent successfully from parent to child.\n");
}

void child_process(const char *pipe_name, const char *output_file) {
    // Open the named pipe for reading
    int fifo_fd = open(pipe_name, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Open the output file for writing
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (output_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from the named pipe and write to the output file
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fifo_fd, buffer, BUFFER_SIZE)) > 0) {
        if (write(output_fd, buffer, bytes_read) != bytes_read) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Close file descriptors
    close(fifo_fd);
    close(output_fd);

    printf("File received successfully by child.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <file_path> <pipe_name> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];
    const char *pipe_name = argv[2];
    const char *output_file = argv[3];

    // Create the named pipe
    if (mkfifo(pipe_name, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        parent_process(file_path, pipe_name);
    } else if (pid == 0) {
        // Child process
        child_process(pipe_name, output_file);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}
