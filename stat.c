#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc, char **argv) {
    // Check if filename/directory name is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file/directory_path>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    struct stat fileStat;

    // Use stat() to get file statistics
    if (stat(filename, &fileStat) < 0) {
        perror("stat");
        return 1;
    }

    // Print file statistics
    printf("Inode number: %ld\n", (long)fileStat.st_ino);
    printf("UID: %d\n", fileStat.st_uid);
    printf("GID: %d\n", fileStat.st_gid);
    printf("File access permissions: %o\n", fileStat.st_mode & 0777);

    // Determine file type
    if (S_ISREG(fileStat.st_mode)) {
        printf("File type: Regular file\n");
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("File type: Directory\n");
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("File type: Symbolic link\n");
    } else if (S_ISFIFO(fileStat.st_mode)) {
        printf("File type: FIFO/pipe\n");
    } else if (S_ISSOCK(fileStat.st_mode)) {
        printf("File type: Socket\n");
    } else if (S_ISCHR(fileStat.st_mode)) {
        printf("File type: Character device\n");
    } else if (S_ISBLK(fileStat.st_mode)) {
        printf("File type: Block device\n");
    } else {
        printf("File type: Unknown\n");
    }

    return 0;
}
In Unix-like systems, file access permissions are represented by a three-digit octal number. Each digit corresponds to a different set of permissions: the first digit represents permissions for the file owner, the second digit represents permissions for the group, and the third digit represents permissions for others.

The permission bits are typically represented as follows:

- 4 (read): The file can be read.
- 2 (write): The file can be written to.
- 1 (execute): The file can be executed (if it's a program or script) or the directory can be entered (if it's a directory).

So, in the context of "664" permission:

- The first digit (6) represents the owner's permissions, which are read (4) and write (2), allowing the owner to read from and write to the file.
- The second digit (6) represents the group's permissions, which are read (4) and write (2), allowing the group to read from and write to the file.
- The third digit (4) represents other users' permissions, which are read (4) only, allowing other users to read from the file but not write to it.

Therefore, the "664" permission means that the owner and group members have read and write permissions, while other users have only read permissions.
