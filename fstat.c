#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char filename[256];
    struct stat fileStat;

    // Prompt the user to enter the file name or directory path
    printf("Enter the file name or directory path: ");
    scanf("%s", filename);

    // Open the file
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Use fstat() to get file statistics
    if (fstat(fd, &fileStat) < 0) {
        printf("Failed to get file statistics.\n");
        close(fd);
        return 1;
    }

    // Close the file descriptor
    close(fd);

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

In Unix-like systems, file permissions are represented by a three-digit octal number. Each digit corresponds to a different set of permissions: the first digit represents permissions for the file owner, the second digit represents permissions for the group, and the third digit represents permissions for others.

The permission bits are typically represented as follows:

4 (read): The file can be read.
2 (write): The file can be written to.
1 (execute): The file can be executed (if it's a program or script) or the directory can be entered (if it's a directory).
So, in the context of "675" permission:

The first digit (6) represents the owner's permissions, which are read (4) and write (2), allowing the owner to read from and write to the file.
The second digit (7) represents the group's permissions, which are read (4), write (2), and execute (1), allowing the group to read from, write to, and execute the file.
The third digit (5) represents other users' permissions, which are read (4) and execute (1), allowing other users to read from and execute the file.
