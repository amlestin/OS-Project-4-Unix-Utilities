#include <stdio.h>
#include <sys/type.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error. Not enough arguments!\n");
        return 1;
    }

    char *path = argv[1];


    return 0;
}
