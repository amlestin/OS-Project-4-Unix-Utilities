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

    // get the path of the directory the program is called from
    char *current_dir = malloc(sizeof(char) * MAXPATHLEN);
    getcwd(current_dir, MAXPATHLEN);

    if (argv[1] == NULL)
    {
        // default to the current directory if the user did not give one
        starting_directory = current_dir;
    }
    else
    {
        starting_directory = argv[1];
    }

    // print the directory the search starts at
    printf("%s\n", starting_directory);

    // recursively print every part of the directory tree
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
        // ignore references to the current and parent directories
        if (de->d_name[0] == '.')
            continue;

        // call the recursive function on entries that are directories
        if (de->d_type == DT_DIR)
        {
                    printf("%s\n", de->d_name);

            // extend the current path with the next directory's name
            char *new_path = calloc(strlen(&slash[0]) + strlen(path_to_dir) + strlen(de->d_name) + 1, sizeof(char));
            strcat(new_path, path_to_dir);
            strcat(new_path, &slash[0]);
            strcat(new_path, de->d_name);

            // have the next directory indent its print statements by 4 more than the parent's
            int new_indent = indent + 4;
            print_until_end(new_path, new_indent);

            free(new_path);
        }
        else
        {
            printf("%s\n", de->d_name);
        }
    }

    closedir(cur_dir);
}
