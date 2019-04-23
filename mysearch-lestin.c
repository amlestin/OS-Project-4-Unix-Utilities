#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: Not enough arguments.\n");
        printf("Usage: ./mysearch [filename]\n");
        return 1;
    }
    char *file_name_to_seek = argv[1];



    return 0;
}