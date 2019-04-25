#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include "strmode.c"

char *path_to_filename(char *path)
{
    char *last_slash;
    char *c;

    int slash_found = 0;
    for (c = path; *c != '\0'; c++)
    {
        if (*c == '/')
        {
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: No file was provided!\n");
        printf("Usage: ./myls [file]\n");
        return 1;
    }

    char *path = argv[1];
    struct stat *buf = malloc(sizeof(struct stat));

    int err = stat(path, buf);
    if (err == -1)
    {
        printf("Error calling stat. Verify file name.\n");
        return 1;
    }

    // File:
    printf("  File: `%s", path_to_filename(path));
    printf("'\n");

    // Size:
    printf("  Size: %lld\t\t", buf->st_size);

    // Blocks:
    printf("Blocks: %lld         ", buf->st_blocks);

    // IO Block
    printf("IO Block: %d  ", buf->st_blksize);

    // type of file
    switch (buf->st_mode & S_IFMT)
    {
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

    // Device:
    int minor = minor(buf->st_dev);
    printf("Device: %xh/%dd ", minor, minor);

    // Inode:
    printf("Inode: %ld\t", (long)buf->st_ino);

    // Links:
    printf("Links: %ld", (long)buf->st_nlink);
    printf("\n");

    // Access: (Ownership)
    char s[13];
    strmode(buf->st_mode, &s[0]);
    printf("Access: (%ho/%s)\t", buf->st_mode, s);

    // Uid:
    struct passwd *cur_pwd = getpwuid(buf->st_uid);
    printf("Uid: (%ld/ %s)\t", (long)buf->st_uid, cur_pwd->pw_name);

    // Gid:
    struct group *grp = getgrgid(buf->st_gid);

    if (grp != NULL) {
        printf("Gid: (%ld/ %s)", (long)buf->st_gid, grp->gr_name);
    }
    else
    { 
        printf("Gid: (%ld/ UNKNOWN)", (long)buf->st_gid);

    }
    printf("\n");

    int buf_size = 512;
    char *a_time_buf = malloc(sizeof(char) * buf_size);
    char *m_time_buf = malloc(sizeof(char) * buf_size);
    char *c_time_buf = malloc(sizeof(char) * buf_size);

    struct tm *a_tm;
    struct tm *m_tm;
    struct tm *c_tm;

    a_tm = localtime(&buf->st_atime);
    m_tm = localtime(&buf->st_mtime);
    c_tm = localtime(&buf->st_ctime);

    strftime(a_time_buf, buf_size, "%F %T %z", a_tm);
    strftime(m_time_buf, buf_size, "%F %T %z", m_tm);
    strftime(c_time_buf, buf_size, "%F %T %z", c_tm);

    // Access:
    printf("Access: %s", a_time_buf);
    printf("\n");
    // Modify:
    printf("Modify: %s", m_time_buf);
    printf("\n");
    // Change:
    printf("Change: %s", c_time_buf);
    printf("\n");

    free(a_time_buf);
    free(m_time_buf);
    free(c_time_buf);

    return 0;
}
