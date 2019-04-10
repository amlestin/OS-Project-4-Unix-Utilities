#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

char *path_to_file_name(char *path) {
    char *last_slash;
    char *c;

    int slash_found = 0;
    for (c = path; *c != '\0'; c++) {
        if (*c == '/') {
            last_slash = c;
            slash_found = 1;
        }
    }
    last_slash++;

    if (slash_found)
        return last_slash;
    else
        return path;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No file was provided!\n");
        printf("Usage: ./myls [file]\n");
        return 1;
    }

    char *path = argv[1];
    struct stat *buf;

    int err = stat(path, buf);
    if (err == -1) {
        printf("Error calling stat. Verify file name.\n");
        return 1;
    }


    // File:
    printf("File: `%s", path_to_file_name(path)); printf("'\n");

    // Size:
    printf("Size: %d\t", buf->st_size);
    printf("\n");

    // Blocks: IO Block:   [type of file]
    // Device:  Inode:  Links:
    // Access:  Uid:    Gid:
    // Access:
    // Modify:
    // Change:


    return 0;
}
