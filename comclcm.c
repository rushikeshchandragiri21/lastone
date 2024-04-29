#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void cat(char *filename) {
    FILE *file;
    int ch;

    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read and print each character from the file
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    // Close the file
    fclose(file);
}

void ls() {
    DIR *dir;
    struct dirent *entry;

    // Open the current directory
    dir = opendir(".");

    // Read and print each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);
}

void cp(char *source, char *destination) {
    FILE *source_file, *destination_file;
    int ch;

    // Open the source file for reading
    source_file = fopen(source, "r");
    if (source_file == NULL) {
        perror("Error opening source file");
        return;
    }

    // Open the destination file for writing
    destination_file = fopen(destination, "w");
    if (destination_file == NULL) {
        perror("Error opening destination file");
        fclose(source_file);
        return;
    }

    // Copy contents from source file to destination file
    while ((ch = fgetc(source_file)) != EOF) {
        fputc(ch, destination_file);
    }

    // Close both files
    fclose(source_file);
    fclose(destination_file);
}

void mv(char *source, char *destination) {
    if (rename(source, destination) != 0) {
        perror("Error moving file");
        return;
    }
}

void head(char *filename) {
    FILE *file;
    int ch, lines = 0;

    // Open the file for reading
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Read and print each character from the file until 10 lines are reached
    while ((ch = fgetc(file)) != EOF && lines < 10) {
        putchar(ch);
        if (ch == '\n') {
            lines++;
        }
    }

    // Close the file
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "cat") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s cat <filename>\n", argv[0]);
            return 1;
        }
        cat(argv[2]);
    } else if (strcmp(argv[1], "ls") == 0) {
        ls();
    } else if (strcmp(argv[1], "cp") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s cp <source_file> <destination_file>\n", argv[0]);
            return 1;
        }
        cp(argv[2], argv[3]);
    } else if (strcmp(argv[1], "mv") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s mv <source_file> <destination_file>\n", argv[0]);
            return 1;
        }
        mv(argv[2], argv[3]);
    } else if (strcmp(argv[1], "head") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s head <filename>\n", argv[0]);
            return 1;
        }
        head(argv[2]);
    } else {
        fprintf(stderr, "Invalid command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
// ./combined_commands cat filename.txt
// ./combined_commands ls
// ./combined_commands cp source.txt destination.txt
// ./combined_commands mv source.txt destination.txt
// ./combined_commands head filename.txt
