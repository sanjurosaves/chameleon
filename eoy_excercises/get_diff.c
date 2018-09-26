#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "dpcm.h"

sample *get_sample(FILE* pcm, int lp, int rp)
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

	if ((lp == -999999) | (rp == -999999))
	{
		newsample->pl = -999999;
		newsample->pr = -999999;
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
sample_diff *get_diff_stereo(sample *current_sample)
{
	sample_diff *newdiff;

	newdiff = malloc(sizeof(sample_diff));
	if (newdiff == NULL)
		return (NULL);

	if (current_sample->pl != -999999)
	{
		newdiff->ldiff = current_sample->l - current_sample->pl;
		newdiff->rdiff = current_sample->r - current_sample->pr;
	}
	else
	{
		newdiff->ldiff = current_sample->l;
		newdiff->rdiff = current_sample->r;
	}

	return (newdiff);
}

int traverse_pcm_file(void)
{
	FILE *pcm, *dpcm;
	unsigned int i;
	sample *current_sample;
	sample_diff *diff;
	int lp = -999999, rp = -999999;

	pcm = fopen("zebraPCMle.pcm", "rb");
	dpcm = fopen("zebra.dpcm", "wb");

	for (i = 1; !(feof(pcm)); i++)
	{
		if (ferror(pcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		current_sample = get_sample(pcm, lp, rp);
		diff = get_diff_stereo(current_sample);
		/* quantized_diff = quantize_diff_stereo(diff); */

		lp = current_sample->l;
		rp = current_sample->r;

		free(current_sample);
		free(diff);
	}

	fclose(pcm);
	fclose(dpcm);

	return (0);
}

int main(void)
{
	return (traverse_pcm_file());
}
