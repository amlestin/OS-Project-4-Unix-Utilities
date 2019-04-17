#include <stdio.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char buf[PATH_MAX];
    char *s = getcwd(&buf[0], PATH_MAX);

    printf("Current directory is %s\n", s);

    return 0;
}