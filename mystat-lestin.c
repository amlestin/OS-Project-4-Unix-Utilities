#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "strmode.c"

char *path_to_filename(char *path) {
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
    struct stat *buf = malloc(sizeof(struct stat));

    int err = stat(path, buf);
    if (err == -1) {
        printf("Error calling stat. Verify file name.\n");
        return 1;
    }


    // File:
    printf("  File: `%s", path_to_filename(path)); printf("'\n");

    // Size:
    printf("  Size: %lld\t\t", buf->st_size);

    // Blocks:
    printf("Blocks: %lld         ", buf->st_blocks);


    // IO Block
    printf("IO Block: %d  ", buf->st_blksize);

    // type of file
    switch(buf->st_mode & S_IFMT) {
        case S_IFBLK:
            printf("block device");
            break;
        case S_IFCHR:
            printf("character device");
            break;
        case S_IFDIR:
            printf("directory");
            break;
        case S_IFIFO:
            printf("FIFO/pipe");
            break;
        case S_IFLNK:
            printf("symlink");
            break;
        case S_IFREG:
            printf("regular file");
            break;
        case S_IFSOCK:
            printf("socket");
            break;
        default:
            printf("unknown");
            break;
    }
    printf("\n");

    //TODO: Is major number needed here
    // Device:
    int minor = minor(buf->st_dev);
    printf("Device: /%dd ", minor);

    // Inode:
    printf("Inode: %ld\t", (long) buf->st_ino);

    // Links:
    printf("Links: %ld", (long) buf->st_nlink);
    printf("\n");

    // Access: (Ownership)
    char s[13];
    strmode(buf->st_mode, &s[0]);

    unsigned long sugo = buf->st_mode & (S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO);
    printf("Ownership: (%lo/%s)\t", sugo, s);
    printf("Ownership: (%ho/%s)\t", buf->st_mode, s);
    // Uid:
    printf("Uid: %ld\t", (long) buf->st_uid);
    // Gid:
    printf("Gid: %ld", (long) buf->st_gid);
    printf("\n");

    // Access:
    printf("Access: %s", ctime(&buf->st_atime));
    // Modify:
    printf("Modify: %s", ctime(&buf->st_mtime));
    // Change:
    printf("Change: %s", ctime(&buf->st_ctime));
    printf("\n");


    // Inode:  Links:
    // Access:  Uid:    Gid:
    // Access:
    // Modify:
    // Change:


    return 0;
}
