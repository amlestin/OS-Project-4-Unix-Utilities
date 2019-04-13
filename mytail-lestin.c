#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Error: Not enough arguments!\n");
		printf("Usage: ./mytail -n [filename]\n");
	}

	printf("Flag: %s\n", argv[1]);
	printf("File: %s\n", argv[2]);

	return 0;
}
