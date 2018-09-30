#include "dpcm.h"

/**
 * reconstruct_sample - decodes representation of original sample
 * original sample from quantized differential sample
 * @dpcm: file pointer to dpcm
 * @lp: left previous sample
 * @rp: right previous sample
 * Return: pointer to reconstructed sample
 */
curr_sample *reconstruct_sample(FILE *dpcm, int lp, int rp)
{
	curr_sample *rs;
	unsigned char compressed_dsample;

	rs = malloc(sizeof(curr_sample));
	if (rs == NULL)
		return (NULL);

	compressed_dsample = fgetc(dpcm);
	if (feof(dpcm))
	{
		free(rs);
		return (NULL);
	}
	rs->l = exponential[(int)compressed_dsample] + lp;
	compressed_dsample = fgetc(dpcm);
	rs->r = exponential[(int)compressed_dsample] + rp;

	if (rs->l > 32767)
		rs->l = 32767;
	if (rs->l < -32768)
		rs->l = -32768;
	if (rs->r > 32767)
		rs->r = 32767;
	if (rs->r < -32768)
		rs->r = -32768;

	return (rs);
}

curr_sample *convert_sample_to_curr_sample(sample *current_sample)
{
	curr_sample *cs;

	cs = malloc(sizeof(curr_sample));
	if (cs == NULL)
		return (NULL);

	cs->l = current_sample->l;
	cs->r = current_sample->r;

	return (cs);
}

/**
 * save_sample - splits each integer channel of a stereo
 * sample into a char and saves to PCM file using fputc
 * @pcm: file pointer to pcm file
 * @decompressed_sample: pointer to stereo sample
 */
void save_sample(FILE *pcm, curr_sample *decompressed_sample)
{
	/*
	 * variables
	 * @hi: high-order (leftmost) byte of 16-bit sample
	 * @lo: low-order (rightmost) byte of 16-bit sample
	 * @shifted: high-order byte expanded to 16-bit val, shifted left 8 bits
	 */

	char hi, lo, i;

	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			hi = decompressed_sample->l >> 8;
			lo = decompressed_sample->l & (0x00ff);
		}
		else
		{
			hi = decompressed_sample->r >> 8;
			lo = decompressed_sample->r & (0x00ff);
		}

		fputc(hi, pcm);
		fputc(lo, pcm);
	}

}

/**
 * get_sample - retrieves stereo sample from PCM file
 * @pcm: file pointer to pcm file
 * @lp: left-previous sample
 * @rp: right-previous sample
 */
sample *get_sample(FILE *pcm, int lp, int rp)
{
	/*
	 * variables
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
		if (feof(pcm))
		{
			free(newsample);
			return (NULL);
		}
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
