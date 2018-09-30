#include "dpcm.h"

/**
 * compress_pcm_to_dpcm - encodes pcm file to dpcm
 * @pcm_path: path to pcm file
 * @dpcm_path: path dpcm file
 * Return: 0 on success, -1 on failure
 */
int compress_pcm_to_dpcm(char *pcm_path, char *dpcm_path)
{
	FILE *pcm, *dpcm;
	unsigned int i = 1;
	sample *current_sample;
	curr_sample *ccs;
	sample_diff *diff;
	quantz_diff *qdif;
	int lp = -999999, rp = -999999, status = 0;

	if (verify_existence(pcm_path) == -1)
	{
		printf("specified PCM file does not exist");
		return (-1);
	}

	pcm = fopen(pcm_path, "rb");
	dpcm = fopen(dpcm_path, "wb");

	for (i = 1; !(feof(pcm)); i++)
	{
		if (ferror(pcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		if (i % 16 == 0)
		{
			/* insertcheckpoint(); */
			current_sample = get_sample(pcm, lp, rp);
			if (current_sample == NULL)
				break;

			lp = current_sample->l;
			rp = current_sample->r;

			ccs = convert_sample_to_curr_sample(current_sample);
			save_sample(dpcm, ccs);
			free(current_sample);
			free(ccs);
		}
		else
		{
			current_sample = get_sample(pcm, lp, rp);
			if (current_sample == NULL)
				break;
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
	}

	fclose(pcm);
	fclose(dpcm);

	return (0);
}

/**
 * decompress_dpcm_to_pcm - decodes dpcm file to pcm
 * @pcm_path: path to pcm file
 * @dpcm_path: path dpcm file
 * Return: 0 on success, -1 on failure
 */
int decompress_dpcm_to_pcm(char *dpcm_path, char *pcm_path)
{
	FILE *dpcm, *pcm;
	curr_sample *decompressed_sample, *ccs;
	sample *current_sample;
	int lp = 0, rp = 0, i;


	if (verify_existence(dpcm_path) == -1)
	{
		printf("specified DPCM file does not exist");
		return (-1);
	}

	dpcm = fopen(dpcm_path, "rb");
	pcm = fopen(pcm_path, "wb");

	for (i = 1; !(feof(dpcm)); i++)
	{
		if (ferror(dpcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		if (i % 16 == 0)
		{
			/* retrieveuncompressedsample(); */
			current_sample = get_sample(dpcm, lp, rp);
			if (current_sample == NULL)
				break;
			lp = current_sample->l;
			rp = current_sample->r;
			ccs = convert_sample_to_curr_sample(current_sample);
			save_sample(pcm, ccs);
			free(current_sample);
			free(ccs);
		}
		else
		{
			decompressed_sample = reconstruct_sample(dpcm, lp, rp);
			if (decompressed_sample == NULL)
				break;
			lp = decompressed_sample->l;
			rp = decompressed_sample->r;
			save_sample(pcm, decompressed_sample);
			free(decompressed_sample);
		}
	}
	fclose(dpcm);
	fclose(pcm);

	return (0);
}
