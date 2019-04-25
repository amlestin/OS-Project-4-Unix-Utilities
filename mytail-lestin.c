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
	if (argc == 4) // if user input is [-n] [number of lines] [filename]
	{
		n = atoi(argv[2]);

		if (n == 0)
		{
			printf("enter a non-zero value for n\n");
			return 1;
		}

		n = n + 1;

		if (argv[3] == NULL)
			printf("Enter a filename\n");

		filename = argv[3];
	}
	else // if user input is [filename]
	{
		n = 10;
		n = n + 1;
		filename = argv[1];
	}

	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Could not open file %s\n", filename);
		return 1;
	}

	// get the stat information from the file and check return value for error
	struct stat buf;
	int stat_error = stat(filename, &buf);
	if (stat_error != 0)
	{
		printf("Could not get stat information for %s\n", filename);
		return 1;
	}

	/// start at the beginning of the file and record the position
	fseek(file, 0, SEEK_SET);
	long original_position = ftell(file);

	int newline_ctr = 0;
	char c;
	int loc;

	// go to the last byte of the file
	fseek(file, -1, SEEK_END);
	while (1)
	{
		// read a character from the file until none are left
		if (fread(&c, 1, 1, file) == 0)
			break;
		// go back one byte to compensate for fread automatically moving forward one
		fseek(file, -1, SEEK_CUR);

		// count the number of newlines and stop after reaching the desired number
		if (c == '\n')
		{
			newline_ctr++;

			if (newline_ctr == n)
			{
				fseek(file, 1, SEEK_CUR);
				break;
			}
		}

		// stop after reading the entire file backward
		loc = ftell(file);
		if (loc == original_position)
			break;

		// go back one byte to read the previous character from the bottom of the file
		fseek(file, -1, SEEK_CUR);
	}

	// validate user input
	if (n > newline_ctr)
	{
		printf("n is greater than the number of newlines in the file.\n");
		return 1;
	}

	// until all desired lines are printed
	while (n > 0)
	{
		// read the next character until none remain then increment file offset by one
		if (fread(&c, 1, 1, file) == 0)
			break;

		printf("%c", c);

		// if a newline has been reached, decrement lines that remain to be printed
		if (c == '\n')
		{
			n--;

			// stop upon reading the last newline
			if (n == 0)
				break;
		}
	}

	fclose(file);
	return 0;
}
