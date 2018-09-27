#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "dpcm.h"

static const int exponential[255] = {
	-65535, -60227, -55349, -50866, -46746, -42959, -39480, -36282, -33343,
	-30642, -28160, -25879, -23783, -21857, -20086, -18459, -16964, -15590,
	-14327, -13167, -12100, -11120, -10220, -9392, -8631, -7932, -7290,
	-6699, -6157, -5658, -5200, -4779, -4392, -4036, -3709, -3409, -3133,
	-2879, -2646, -2431, -2235, -2054, -1887, -1734, -1594, -1465, -1346,
	-1237, -1137, -1045, -960, -883, -811, -746, -685, -630, -579, -532,
	-489, -449, -413, -380, -349, -321, -295, -271, -249, -229, -210, -193,
	-178, -163, -150, -138, -127, -117, -107, -99, -91, -83, -77, -70, -65,
	-60, -55, -50, -46, -43, -39, -36, -33, -31, -28, -26, -24, -22, -20,
	-19, -17, -16, -15, -13, -12, -11, -11, -10, -9, -8, -8, -7, -7, -6, -6,
	-5, -5, -4, -4, -4, -4, -3, -3, -3, -3, -3, -2, -2, -2, -2, 0, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 9, 10, 10, 11,
	12, 14, 15, 16, 18, 19, 21, 23, 25, 27, 30, 32, 35, 38, 42, 45, 49, 54,
	59, 64, 69, 76, 82, 90, 98, 106, 116, 126, 137, 149, 162, 177, 192, 209,
	228, 248, 270, 294, 320, 348, 379, 412, 448, 488, 531, 578, 629, 684,
	745, 810, 882, 959, 1044, 1136, 1236, 1345, 1464, 1593, 1733, 1886,
	2053, 2234, 2430, 2645, 2878, 3132, 3408, 3708, 4035, 4391, 4778, 5199,
	5657, 6156, 6698, 7289, 7931, 8630, 9391, 10219, 11119, 12099, 13166,
	14326, 15589, 16963, 18458, 20085, 21856, 23782, 25878, 28159, 30641,
	33342, 36281, 39479, 42958, 46745, 50865, 55348, 60226 };

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

	newsample->pl = lp;
	newsample->pr = rp;

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

qdiff *quantize_diff_stereo(sample_diff *diff)
{
	qdiff *newqdiff;

	newqdiff = malloc(sizeof(newqdiff));
	if (nqdiff == NULL)
		return (NULL);

	/* insert binary search of exponential encoding map here */

	return (newqdiff);
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
