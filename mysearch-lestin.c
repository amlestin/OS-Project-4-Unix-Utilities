#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void print_until_end(char *path_to_dir, int indent);

int main(int argc, char *argv[])
{
    char *starting_directory;

    char *current_dir = malloc(sizeof(char) * MAXPATHLEN);
    getcwd(current_dir, MAXPATHLEN);

    if (argv[1] == NULL)
    {
        starting_directory = current_dir;
    }
    else
    {
        starting_directory = argv[1];
    }

    printf("%s\n", starting_directory);

    print_until_end(starting_directory, 0);

    free(current_dir);
    return 0;
}

void print_until_end(char *path_to_dir, int indent)
{
    char slash[2] = "/";
    DIR *cur_dir = opendir(path_to_dir);
    if (cur_dir == NULL)
    {
        printf("Cannot open directory: %s\n", path_to_dir);
        return;
    }

    struct dirent *de;

    while ((de = readdir(cur_dir)) != NULL)
    {
        if (de->d_name[0] == '.')
            continue;

        if (de->d_type == DT_DIR)
        {
            for (int i = 0; i < indent; i++)
            {
                printf(" ");
            }
            printf("├");
            printf("──%s\n", de->d_name);

            char slash[2] = "/";
            char *new_path = calloc(strlen(&slash[0]) + strlen(path_to_dir) + strlen(de->d_name) + 1, sizeof(char));
            strcat(new_path, path_to_dir);
            strcat(new_path, &slash[0]);
            strcat(new_path, de->d_name);

            int new_indent = indent + 4;
            print_until_end(new_path, new_indent);

            free(new_path);
        }
        else
        {
            for (int i = 0; i < indent; i++)
            {
                printf(" ");
            }

            printf("├");
            printf("──%s\n", de->d_name);
        }
    }

    closedir(cur_dir);
}