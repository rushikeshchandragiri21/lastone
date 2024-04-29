#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define FIFO_NAME "uppercase_to_lowercase_fifo"

int main() {
    int fd;
    pid_t pid;

    // Create the named pipe
    if (mknod(FIFO_NAME, S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent process
        char input[100];

        // Open the named pipe for writing
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read input from user
        printf("Enter text: ");
        fgets(input, sizeof(input), stdin);

        // Write input to the named pipe
        write(fd, input, sizeof(input));

        // Close the pipe
        close(fd);
    } else { // Child process
        char buffer[100];

        // Open the named pipe for reading
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read from the named pipe
        read(fd, buffer, sizeof(buffer));

        // Convert uppercase characters to lowercase
        for (int i = 0; buffer[i]; i++) {
            if (isupper(buffer[i])) {
                buffer[i] = tolower(buffer[i]);
            }
        }

        // Write the result to standard output
        printf("Converted text: %s\n", buffer);

        // Close the pipe
        close(fd);
    }

    // Remove the named pipe
    unlink(FIFO_NAME);

    return 0;
}
