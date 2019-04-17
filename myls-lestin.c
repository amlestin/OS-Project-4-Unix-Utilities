#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
/*
    char buf[PATH_MAX];
    char *cur_dir_path = getcwd(&buf[0], PATH_MAX);
    printf("Current directory is %s\n", cur_dir_path);

    DIR *cur_dir = opendir(cur_dir_path);
*/

    DIR *cur_dir = opendir(".");
    struct dirent *de;
    struct stat buf;
    struct group *cur_de_grp;
    struct passwd *cur_de_pwd;

    while ( (de = readdir(cur_dir)) != NULL ) {
        // skip hidden files
        if (de->d_name[0] == '.')
            continue;

        lstat(de->d_name, &buf);

        // User ID of owner
        cur_de_pwd = getpwuid(buf.st_uid);
        printf("%s ", cur_de_pwd->pw_name);

        // Group ID of owner
        cur_de_grp = getgrgid(buf.st_gid);
        printf("%s ", cur_de_grp->gr_name);

        // Size
        printf("%lld ", buf.st_size);

        printf("%s\n", de->d_name);
    }

    closedir(cur_dir);
    return 0;
}