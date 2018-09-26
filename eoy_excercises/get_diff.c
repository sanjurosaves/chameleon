#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "dpcm.h"

sample *get_sample(FILE* pcm)
{
	/* variables
	 * @hi: high-order (leftmost) byte of 16-bit sample
	 * @lo: low-order (rightmost) byte of 16-bit sample
	 * @shifted: high-order byte expanded to 16-bit val, shifted left 8 bits
	 */

	sample *newsample;
	char hi, lo, i;
	short int shifted;

	newsample = malloc(sizeof(sample));
	if (newsample == NULL)
		return (NULL);

	for (i = 0; i < 2; i++)
	{
		hi = fgetc(pcm);
		lo = fgetc(pcm);
		shifted = (((short)hi) << 8);
		if (i == 0)
			newsample->l = shifted | (0x00ff & lo);
		else
			newsample->r = shifted | (0x00ff & lo);
	}

	return (newsample);
}

/**
 * get_diff_stereo - program to dump out each 16-bit sample of a "raw",
 * "uncompressed" stereo PCM file, convert the binary data to a signed 2's
 * complement integer, and calculate and store in a csv file the difference
 * between the integer values of each successive sample.
 * @current: pointer to array of arguments
 * @previous: count of arguments
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
	sample *current_sample; /*, *previous_sample;*/

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
	}

	current_sample = get_sample(pcm);
	free(current_sample);

	return (0);
}

int main(void)
{
	return (traverse_pcm_file());
}
