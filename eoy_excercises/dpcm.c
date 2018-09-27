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

int traverse_pcm_file(void)
{
	FILE *pcm, *dpcm;
	unsigned int i;
	sample *current_sample;
	sample_diff *diff;
	quantz_diff *qdif;
	int lp = -999999, rp = -999999, status = 0;

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
		qdif = quantize_diff_stereo(diff);
		status = save_qdif(dpcm, qdif);
		if (status == -1)
			return (-1);

		lp = current_sample->l;
		rp = current_sample->r;

		free(current_sample);
		free(diff);
		free(qdif);
	}

	fclose(pcm);
	fclose(dpcm);

	return (0);
}

int decompress_dpcm_to_pcm(void)
{
	FILE *dpcm, *pcm;
	char compressed_sample[1];
	int decompressed_sample[1];
	int i;

	dpcm = fopen("zebra.dpcm", "rb");
	pcm = fopen("newzebra.pcm", "wb");

	for (i = 1; !(feof(dpcm)); i++)
	{
		compressed_sample[0] = fgetc(dpcm);
		if (ferror(dpcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}
		decompressed_sample[0] = exponential[(int)compressed_sample[0]];
		fwrite(decompressed_sample, 4, 1, pcm);
	}
	fclose(dpcm);
	fclose(pcm);

	return (0);
}
int main(void)
{
	return (decompress_dpcm_to_pcm());
}
