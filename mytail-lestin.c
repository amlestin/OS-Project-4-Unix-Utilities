#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Error: Not enough arguments!\n");
		printf("Usage: ./mytail -n [number of lines] [filename]\n");
		return 1;
	}

	int n;
	char *filename;
	if (argc == 4)
	{
		n = atoi(argv[2]);
		n = n + 1;
		filename = argv[3];
	}
	else
	{
		n = 10;
		n = n + 1;
		filename = argv[1];
	}

	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return 1;

	struct stat buf;
	int stat_error = stat(filename, &buf);
	if (stat_error != 0)
		return 1;

	int file_size = buf.st_size;

	fseek(file, 0, SEEK_SET);
	long original_position = ftell(file);

	int newline_ctr = 0;
	char c;
	fseek(file, -1, SEEK_END);
	int loc = ftell(file);
	while (1)
	{
		if (fread(&c, 1, 1, file) == 0)
			break;
		fseek(file, -1, SEEK_CUR);

		loc = ftell(file);

		if (c == '\n')
		{
			newline_ctr++;

			if (newline_ctr == n)
			{
				fseek(file, 1, SEEK_CUR);
				break;
			}
		}

		if (loc == original_position)
			break;

		fseek(file, -1, SEEK_CUR);

		loc = ftell(file);
	}

	if (n > newline_ctr)
	{
		printf("n is greater than the number of newlines in the file.\n");
		return 1;
	}

	while (n > 0)
	{
		if (fread(&c, 1, 1, file) == 0)
			break;

		loc = ftell(file);
		printf("%c", c);

		if (c == '\n')
		{
			n--;
			if (n == 0)
				break;
		}
	}

	fclose(file);
	return 0;
}
