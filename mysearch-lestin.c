#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void print_until_end(char *path_to_dir, int indent);

int main(int argc, char *argv[]) {
    char *starting_directory;
    char default_directory[2] = ".";
    if (argv[1] == NULL) {
        starting_directory = &default_directory[0];
    } else {
        starting_directory = argv[1];
    }

    printf("%s\n", starting_directory);

    print_until_end(starting_directory, 0);

    return 0;
}

void print_until_end(char *path_to_dir, int indent) {
    char slash[2] = "/";
    DIR *cur_dir = opendir(path_to_dir);
    struct dirent *de;

    while ( (de = readdir(cur_dir)) != NULL) {
        if (de->d_name[0] == '.')
            continue;

        if (de->d_type == DT_DIR) {
            for (int i = 0; i < indent; i++) {
                printf(" ");
            }
            printf("├");
            printf("──%s\n", de->d_name);

            char *new_path = calloc(strlen(path_to_dir) + strlen(&slash[0]) + strlen(de->d_name) + 1, sizeof(char));
            strcat(new_path, path_to_dir);
            strcat(new_path, &slash[0]);
            strcat(new_path, de->d_name);

            int new_indent = indent + 1;
            print_until_end(new_path, new_indent);

            free(new_path);
        } else {
            for (int i = 0; i < indent; i++) {
                printf(" ");
            }

            printf("├");
            printf("──%s\n", de->d_name);
        }
    }
}