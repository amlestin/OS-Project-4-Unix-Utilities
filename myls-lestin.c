#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "strmode.c"

int main(int argc, char *argv[])
{
    int l_flag = 0;
    int dir_flag = 0;

    // -l flag is first argument
    if (argv[1] != NULL && strcmp(argv[1], "-l") == 0)
    {
        l_flag = 1;
    }

    char *dir_path;
    DIR *cur_dir;

    if (!l_flag && argv[1] != NULL)
    { // no -l flag and a first argument (directory)
        dir_path = argv[1];
        cur_dir = opendir(dir_path);

        if (cur_dir == NULL)
        {
            printf("Invalid directory.\n");
            return 1;
        }
    }
    else if (l_flag && argv[2] != NULL)
    { // l flag and a second argument (directory)
        dir_path = argv[2];
        cur_dir = opendir(dir_path);

        if (cur_dir == NULL)
        {
            printf("Invalid directory.\n");
            return 1;
        }
    }
    else if (l_flag && argv[2] == NULL)
    { // l flag but not second argument (directory)
        char dp[2] = ".";

        dir_path = &dp[0];
        cur_dir = opendir(dir_path);
    }
    else
    { // no flag and no second argument (directory)
        char dp[2] = ".";

        dir_path = &dp[0];
        cur_dir = opendir(dir_path);
    }

    struct dirent *de;
    struct stat buf;
    struct group *cur_de_grp;
    struct passwd *cur_de_pwd;
    char s[13];

    char real_path_buf[PATH_MAX];
    realpath(dir_path, &real_path_buf[0]);
    char slash[2] = "/";

    if (l_flag == 1)
    {
        int total_file_sizes = 0;
        while ((de = readdir(cur_dir)) != NULL)
        {
            // skip hidden files
            if (de->d_name[0] == '.')
                continue;

            char *de_absolute_path = calloc(strlen(real_path_buf) + strlen(&slash[0]) + strlen(de->d_name) + 1, sizeof(char));
            strcat(de_absolute_path, real_path_buf);
            strcat(de_absolute_path, &slash[0]);
            strcat(de_absolute_path, de->d_name);

            lstat(de_absolute_path, &buf);

            total_file_sizes += buf.st_blocks;

            free(de_absolute_path);
        }
        printf("total %d\n", total_file_sizes / 2); // ls uses half the default block size for stat
    }

    rewinddir(cur_dir);
    while ((de = readdir(cur_dir)) != NULL)
    {
        // skip hidden files
        if (de->d_name[0] == '.')
            continue;

        if (l_flag == 1)
        {
            char *de_absolute_path = calloc(strlen(real_path_buf) + strlen(&slash[0]) + strlen(de->d_name) + 1, sizeof(char));
            strcat(de_absolute_path, real_path_buf);
            strcat(de_absolute_path, &slash[0]);
            strcat(de_absolute_path, de->d_name);

            lstat(de_absolute_path, &buf);

            // Ownership
            strmode(buf.st_mode, &s[0]);
            printf("%s ", s);
            // could not print "." or "+" character (SElinux file context) because I could not link the library libselinux with necessary function getfilecon()

            // Number of links
            printf("%d ", buf.st_nlink);

            // User ID of owner
            cur_de_pwd = getpwuid(buf.st_uid);
            printf("%s ", cur_de_pwd->pw_name);

            // Group ID of owner
            printf("%ld ", (long)buf.st_gid);

            // Size
            printf("%5lld ", buf.st_size);
            free(de_absolute_path);

            // Time
            int buf_size = 690;
            char *time_buf = malloc(sizeof(char) * buf_size);

            struct tm *tm;
            tm = localtime(&buf.st_ctime);

            strftime(time_buf, buf_size, "%b %d %H:%M", tm);

            printf("%s ", time_buf);
            free(time_buf);
        }
        if (l_flag == 1)
        {
            printf("%s\n", de->d_name);
        }
        else
        {
            printf("%s\t", de->d_name);
        }
    }
    if (!l_flag)
        printf("\n");

    closedir(cur_dir);
    return 0;
}
