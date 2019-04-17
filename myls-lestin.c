#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include "strmode.c"

int main(int argc, char *argv[]) {
/*
    char buf[PATH_MAX];
    char *cur_dir_path = getcwd(&buf[0], PATH_MAX);
    printf("Current directory is %s\n", cur_dir_path);

    DIR *cur_dir = opendir(cur_dir_path);
*/
    int l_flag = 0;
    int dir_flag = 0;

    if (argv[1] != NULL && strcmp(argv[1], "-l") == 0) {
        printf("%s\n", argv[1]);
        l_flag = 1;
    }

    char *dir_path;
    DIR *cur_dir;
    if (!l_flag && argv[1] != NULL) {
        dir_path = argv[1];
        cur_dir = opendir(dir_path);

        if (cur_dir == NULL) {
            printf("Invalid directory.\n");
            return 1;
        }
    } else if (l_flag && argv[2] != NULL) {
        dir_path = argv[2];
        cur_dir = opendir(dir_path);

        if (cur_dir == NULL) {
            printf("Invalid directory.\n");
            return 1;
        }
    }
    else  {
//        dir_path = malloc(sizeof(char));
        char dp;
        dp = '.';

        dir_path = &dp;
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

    while ( (de = readdir(cur_dir)) != NULL ) {
        // skip hidden files
        if (de->d_name[0] == '.')
            continue;

        if (l_flag == 1) {
            char *de_absolute_path = calloc(strlen(real_path_buf) + strlen(&slash[0]) + strlen(de->d_name) + 1, sizeof(char));
            strcat(de_absolute_path, real_path_buf);
            strcat(de_absolute_path, &slash[0]);
            strcat(de_absolute_path, de->d_name);

            lstat(de_absolute_path, &buf);

            strmode(buf.st_mode, &s[0]);
            // Ownership
            printf("%s ", s);

            // Number of links
            printf("%d ", buf.st_nlink);

            // User ID of owner
            cur_de_pwd = getpwuid(buf.st_uid);
            printf("%s ", cur_de_pwd->pw_name);

            // Group ID of owner
            cur_de_grp = getgrgid(buf.st_gid);
            printf("%s ", cur_de_grp->gr_name);

            // Size
            printf("%8lld ", buf.st_size);
            free(de_absolute_path);
        }
        printf("%s\n", de->d_name);
    }

    closedir(cur_dir);
    return 0;
}