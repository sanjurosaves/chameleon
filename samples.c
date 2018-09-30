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

/**
 * convert_sample_to_curr_sample - converts
 * 4-ch sample type to 2-ch curr-sample type
 * @current_sample: pointer to sample type
 * Return: pointer to 2-ch curr_sample type
 */
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
 * Return: pointer to 4-ch sample; NULL on failure
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
