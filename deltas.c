#include "dpcm.h"

/**
 * get_diff_stereo - program to dump out each 16-bit sample of a "raw",
 * "uncompressed" stereo PCM file, convert the binary data to a signed 2's
 * complement integer, and calculate and store in a csv file the difference
 * between the integer values of each successive sample.
 * @current_sample: pointer to current sample
 * Return: difference b/w sample value and previous sample value, NULL on error
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

/**
 * quantz_diff - quantizes differential between two samples
 * @diff: pointer to full-sized differential sample
 * Return: pointer to quantized differential sample
 */
quantz_diff *quantize_diff_stereo(sample_diff *diff)
{
	quantz_diff *newqdiff;

	newqdiff = malloc(sizeof(quantz_diff));
	if (newqdiff == NULL)
		return (NULL);

	newqdiff->ldiff = bsearcharray(diff->ldiff, 0, 255);
	newqdiff->rdiff = bsearcharray(diff->rdiff, 0, 255);

	return (newqdiff);
}

/**
 * save_qdif - saves quantized differential to dpcm file
 * @dpcm: file pointer to dpcm file
 * @qdif: pointer to quantized differential structure
 * Return: 0 on sucess, 1 on failure
 */
int save_qdif(FILE *dpcm, quantz_diff *qdif)
{
	int rval;

	rval = fputc(qdif->ldiff, dpcm);
	if (rval != qdif->ldiff)
	{
		printf("Error saving quantized sample difference");
		return (-1);
	}
	rval = fputc(qdif->rdiff, dpcm);
	if (rval != qdif->rdiff)
	{
		printf("Error saving quantized sample difference");
		return (-1);
	}

	return (0);
}
