#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>

/**
 * main - program to dump out each 16-bit sample of a "raw",
 * "uncompressed" stereo PCM file, convert the binary data to a signed 2's
 * complement integer, and calculate and store in a csv file the value of each
 * sample, as well as the difference between the values of each sample.
 * @argv: pointer to array of arguments
 * @argc: count of arguments
 * Return: 0 upon success, 1 upon error
 */
int main(int argc, char **argv)
{
	/**
	 * variables:
	 * a: leftmost 8-bits of 16-bit sample
	 * b: rightmost 8-bits of 16-bit sample
	 * i: loop iterator/joined stereo sample number
	 * l: value of left-channel sample
	 * r: value of right-channel sample
	 * pl: value of previous left-channel sample
	 * pr: value of previous right-channel sample
	 * shifted: a-byte expanded to 16-bit value and shifted right 8 bits
	*/
	FILE *csv, *pcm;
	char a, b;
	int i, l, r, pl, pr;
	short int shifteda;

	if (argc != 3)
	{
		perror("Usage: [INPUT PCM FILE] [OUTPUT CSV FILE]\n");
		return (1);
	}

	csv = fopen(argv[2], "w");
	if (csv == NULL)
	{
		printf("Error opening csv file\n");
		return (1);
	}
	pcm = fopen(argv[1], "rb");
	if (pcm == NULL)
	{
		printf("Error opening pcm file\n");
		return (1);
	}

	fprintf(csv, "sample #,left val,dist from prev left val,");
	fprintf(csv, "right val,dist from prev rval\n");

	for (i = 1; !(feof(pcm)); i++)
	{
		if (ferror(pcm))
		{
			int en = errno;

			printf("error at sample %d\n", i);
			printf("Error number: %d\n", en);
			break;
		}
		a = fgetc(pcm);
		b = fgetc(pcm);
		shifteda = (((short)a) << 8);
		l = shifteda | (0x00ff & b);
		a = fgetc(pcm);
		b = fgetc(pcm);
		shifteda = (((short)a) << 8);
		r = shifteda | (0x00ff & b);

		if (i == 1)
			fprintf(csv, "%d,%d,%d,%d,%d\n", i, l, 0, r, 0);
		else
			fprintf(csv, "%d,%d,%d,%d,%d\n", i, l, (l - pl), r, (r - pr));
		pl = l;
		pr = r;
	}
	fclose(csv);
	fclose(pcm);
	return (0);
}
