#include "dpcm.h"
#include "adpcm.h"

/**
 * get_diff_stereo - program to dump out each 16-bit sample of a "raw",
 * "uncompressed" stereo PCM file, convert the binary data to a signed 2's
 * complement integer, and calculate and store in a csv file the difference
 * between the integer values of each successive sample.
 * @current_sample: pointer to current sample
 * Return: difference b/w sample value and previous sample value, NULL on error
 */
sample_diff *get_diff_stereo(curr_sample *current_sample, int lp, int rp)
{
	sample_diff *newdiff;

	newdiff = malloc(sizeof(sample_diff));
	if (newdiff == NULL)
		return (NULL);

	if ((lp != -999999) && (rp != -999999))
	{
		newdiff->ldiff = current_sample->l - lp;
		newdiff->rdiff = current_sample->r - rp;
	}
	else
	{
		newdiff->ldiff = current_sample->l;
		newdiff->rdiff = current_sample->r;
	}

	if (newdiff->ldiff < 0)
	{
		newdiff->ldiff = -(newdiff->ldiff);
		newdiff->l_s_b = 1;
	}
	else
		newdiff->l_s_b = 0;

	if (newdiff->rdiff < 0)
	{
		newdiff->rdiff = -(newdiff->rdiff);
		newdiff->r_s_b = 1;
	}
	else
		newdiff->r_s_b = 0;

/*	printf("ldelta: %d, rdelta: %d\n", newdiff->ldiff, newdiff->rdiff);
	printf("l_s_b: %d, r_s_b: %d\n", newdiff->l_s_b, newdiff->r_s_b);*/

	return (newdiff);
}

/**
 * quantz_diff - quantizes differential between two samples
 * @diff: pointer to full-sized differential sample
 * Return: pointer to quantized differential sample
 */
quantz_diff *quantize_diff_stereo(sample_diff *diff, int lp, int rp)
{
	quantz_diff *newqdiff;
	unsigned char code;
	short int index = 0;
	int predicted_delta;

	newqdiff = malloc(sizeof(quantz_diff));
	if (newqdiff == NULL)
		return (NULL);

	code = 4 * diff->ldiff / ima_step_size[index];
	if (code > 7)
		code = 7;
/*	printf("code: %d\n", code);*/

	index += ima_index_adjust[code];
	if (index < 0)
		index = 0;
	if (index > 88)
		index = 88;
/*	printf("index: %d\n", index);*/

	predicted_delta = (ima_step_size[index] * code) / 4
		+ (ima_step_size[index] / 8);
/*	printf("predicted_delta: %d\n", predicted_delta);*/

	lp += predicted_delta;
	if (lp > 32767)
		lp = 32767;
	if (lp < -32768)
		lp = -32768;
/*	printf("prev_sample: %d\n", lp);*/

	newqdiff->byte_l = code + diff->l_s_b * 8;
/*	printf("save code: %d\n", newqdiff->byte_l);*/

	code = 4 * diff->rdiff / ima_step_size[index];
	if (code > 7)
		code = 7;

	index += ima_index_adjust[code];
	if (index < 0)
		index = 0;
	if (index > 88)
		index = 88;

	predicted_delta = (ima_step_size[index] * code) / 4
		+ (ima_step_size[index] / 8);

	rp += predicted_delta;
	if (rp > 32767)
		rp = 32767;
	if (rp < -32768)
		rp = -32768;

	newqdiff->byte_r = code + diff->r_s_b * 8;

	return (newqdiff);
}

/**
 * save_qdif - saves quantized differential to dpcm file
 * @adpcm: file pointer to adpcm file
 * @qdif: pointer to quantized differential structure
 * Return: 0 on sucess, 1 on failure
 */
int save_qdif(FILE *adpcm, quantz_diff *qdif)
{
	int rval;
	unsigned char byte;

	byte = (qdif->byte_l << 4) | qdif->byte_r;

	rval = fputc(byte, adpcm);
	if (rval != byte)
	{
		printf("Error saving quantized sample difference");
		return (-1);
	}

	return (0);
}
