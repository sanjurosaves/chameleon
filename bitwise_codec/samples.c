#include "dpcm.h"
#include "adpcm.h"

/**
 * reconstruct_sample - decodes representation of original sample
 * original sample from quantized differential sample
 * @adpcm: file pointer to dpcm
 * @lp: left previous sample
 * @rp: right previous sample
 * Return: pointer to reconstructed sample
 */
curr_sample *reconstruct_sample(FILE *adpcm)
{
	curr_sample *rs;
	unsigned char compressed_sample;
	unsigned char l_byte, r_byte;
	unsigned short int sign_bit;
	short int code, l_index = 0, r_index = 0, i;
	int delta;

	rs = malloc(sizeof(curr_sample));
	if (rs == NULL)
		return (NULL);
	rs->l = 0;
	rs->r = 0;

	compressed_sample = fgetc(adpcm);
	if (feof(adpcm))
	{
		free(rs);
		return (NULL);
	}

	l_byte = compressed_sample >> 4;
	r_byte = compressed_sample & 0x000f;
/*	printf("r_byte: %d", r_byte);*/

	for (i = 0; i < 2; i++)
	{
		if (i % 2 == 0)
			code = l_byte;
		else
			code = r_byte;

		if (code & (1 << 3))
			sign_bit = 1;
		else
			sign_bit = 0;
		code = ~(1 << 3) & code;
/*		printf("code: %d\n", code);*/

		if (i % 2 == 0)
			delta = ((ima_step_size[l_index] * code) / 4)
			+ (ima_step_size[l_index] / 8);
		else
			delta = ((ima_step_size[r_index] * code) / 4)
			+ (ima_step_size[r_index] / 8);

		if (sign_bit == 1)
			delta = -(delta);
/*		printf("delta: %d\n", delta);*/

		if (i % 2 == 0)
			rs->l += delta;
		else
			rs->r += delta;

		if (i % 2 == 0)
		{
			l_index += ima_index_adjust[code];
			if (l_index < 0)
				l_index = 0;
			if (l_index > 88)
				l_index = 88;
		}
		else
		{
			r_index += ima_index_adjust[code];
			if (r_index < 0)
				r_index = 0;
			if (r_index > 88)
				r_index = 88;
		}
	}

	if (rs->l > 32767)
		rs->l = 32767;
	else if (rs->l < -32768)
		rs->l = -32768;
/*	printf("lcur_sample: %d\n", rs->l);*/

	if (rs->r > 32767)
		rs->r = 32767;
	else if (rs->r < -32768)
		rs->r = -32768;
/*	printf("rcur_sample: %d\n", rs->r);*/

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

	unsigned char hi, lo, i;

	for (i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			hi = decompressed_sample->l >> 8;
			lo = decompressed_sample->l & (0x00ff);
			/* printf("hi: %d lo: %d\n", hi, lo); */
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
curr_sample *get_sample(FILE *pcm)
{
	/*
	 * variables
	 * @hi: high-order (leftmost) byte of 16-bit sample
	 * @lo: low-order (rightmost) byte of 16-bit sample
	 * @shifted: high-order byte expanded to 16-bit val, shifted left 8 bits
	 */

	curr_sample *newsample;
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
		if (feof(pcm))
		{
			free(newsample);
			return (NULL);
		}
		shifted = (((short)hi) << 8);
		if (i == 0)
			newsample->l = shifted | (0x00ff & lo);
		else
			newsample->r = shifted | (0x00ff & lo);
	}

	return (newsample);
}
