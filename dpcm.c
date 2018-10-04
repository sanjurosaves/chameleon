#include "dpcm.h"
#include "adpcm.h"

/**
 * compress_pcm_to_adpcm - encodes pcm file to adpcm
 * @pcm_path: path to pcm file
 * @adpcm_path: path adpcm file
 * Return: 0 on success, -1 on failure
 */
int compress_pcm_to_adpcm(char *pcm_path, char *adpcm_path)
{
	FILE *pcm, *adpcm;
	unsigned int i = 1;
	curr_sample *current_sample;
	sample_diff *delta;
	quantz_diff *qdif;
	int lp = 0, rp = 0, status = 0;

	if (verify_existence(pcm_path) == -1)
	{
		printf("specified PCM file does not exist");
		return (-1);
	}

	pcm = fopen(pcm_path, "rb");
	adpcm = fopen(adpcm_path, "wb");

	for (i = 1; !(feof(pcm)); i++)
	{
		if (ferror(pcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		if ((i % 16 == 0) || (i == 1))
		{
			current_sample = get_sample(pcm);
			if (current_sample == NULL)
				break;

			lp = current_sample->l;
			rp = current_sample->r;

			save_sample(adpcm, current_sample);
			free(current_sample);
		}
		else
		{
			current_sample = get_sample(pcm);
			if (current_sample == NULL)
				break;
/*			printf("l: %d, r: %d\n", current_sample->l, current_sample->r);*/
			delta = get_diff_stereo(current_sample, lp, rp);
			qdif = quantize_diff_stereo(delta, lp, rp);
			status = save_qdif(adpcm, qdif);
			if (status == -1)
				return (-1);

			lp = current_sample->l;
			rp = current_sample->r;

			free(current_sample);
			free(delta);
			free(qdif);
		}
	}
	printf("i: %d\n", i);
	fclose(pcm);
	fclose(adpcm);

	return (0);
}

/**
 * decompress_adpcm_to_pcm - decodes adpcm file to pcm
 * @pcm_path: path to pcm file
 * @adpcm_path: path adpcm file
 * Return: 0 on success, -1 on failure
 */
int decompress_adpcm_to_pcm(char *adpcm_path, char *pcm_path)
{
	FILE *adpcm, *pcm;
	curr_sample *decompressed_sample, *current_sample;
	unsigned int i;

	if (verify_existence(adpcm_path) == -1)
	{
		printf("specified DPCM file does not exist");
		return (-1);
	}

	adpcm = fopen(adpcm_path, "rb");
	pcm = fopen(pcm_path, "wb");

	for (i = 1; !(feof(adpcm)); i++)
	{
		if (ferror(adpcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		if ((i % 16 == 0) || (i == 1))
		{
			current_sample = get_sample(adpcm);
			if (current_sample == NULL)
				break;

			save_sample(pcm, current_sample);
			free(current_sample);
		}
		else
		{
			decompressed_sample = reconstruct_sample(adpcm);
			if (decompressed_sample == NULL)
				break;
			save_sample(pcm, decompressed_sample);
			free(decompressed_sample);
		}
	}
	fclose(adpcm);
	fclose(pcm);

	return (0);
}
