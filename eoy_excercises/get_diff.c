#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>

/**
 * get_diff - program to dump out each 16-bit sample of a "raw",
 * "uncompressed" stereo PCM file, convert the binary data to a signed 2's
 * complement integer, and calculate and store in a csv file the difference
 * between the integer values of each successive sample.
 * @argv: pointer to array of arguments
 * @argc: count of arguments
 * Return: difference b/w sample value and previous sample value, -1 upon error
 */
int get_diff_stereo(int current, int previous)
{
	int diff = current - previous;
	/*
	a = fgetc(pcm);
		b = fgetc(pcm);
		shifteda = (((short)a) << 8);
		l = shifteda | (0x00ff & b);
		a = fgetc(pcm);
		b = fgetc(pcm);
		shifteda = (((short)a) << 8);
		r = shifteda | (0x00ff & b);

		if (i == 1)
			printf("%d,%d,%d\n", i, l, r);
		else
			printf("%d,%d,%d\n", i, (l - pl), (r - pr));
		pl = l;
		pr = r;
	*/
	return (diff);
}

int traverse_pcm_file(void)
{
	FILE* pcm;
	unsigned int i;

	pcm = fopen("zebraPCMle.pcm", "rb");

	for (i = 1; !(feof(pcm)); i++)
	{
		if (ferror(pcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		fgetc(pcm);
	}

	fclose(pcm);

	return (0);
}

int main(void)
{
	return (traverse_pcm_file());
}
