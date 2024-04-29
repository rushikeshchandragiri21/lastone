#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process - read from pipe, convert to lowercase, and print
        close(pipe_fd[1]); // Close write end of the pipe

        char ch;
        while (read(pipe_fd[0], &ch, 1) > 0) {
            putchar(tolower(ch));
        }

        close(pipe_fd[0]); // Close read end of the pipe
        fclose(file);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process - read from file, write to pipe
        close(pipe_fd[0]); // Close read end of the pipe

        char ch;
        while ((ch = fgetc(file)) != EOF) {
            if (write(pipe_fd[1], &ch, 1) != 1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipe_fd[1]); // Close write end of the pipe
        fclose(file);

        // Wait for child process to finish
        wait(NULL);
    }

    return 0;
}

