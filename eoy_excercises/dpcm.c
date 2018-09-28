#include "dpcm.h"

int compress_pcm_to_dpcm(char *pcm_path, char *dpcm_path)
{
	FILE *pcm, *dpcm;
	unsigned int i = 1;
	sample *current_sample;
	sample_diff *diff;
	quantz_diff *qdif;
	int lp = -999999, rp = -999999, status = 0;

	if (verify_existance(pcm_path) == -1)
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

	fclose(pcm);
	fclose(dpcm);

	return (0);
}

int decompress_dpcm_to_pcm(char *dpcm_path, char *pcm_path)
{
	FILE *dpcm, *pcm;
	/* char compressed_sample; */
	curr_sample *decompressed_sample;
	int lp, rp, i, index;

	if (verify_existance(dpcm_path) == -1)
	{
		printf("specified DPCM file does not exist");
		return (-1);
	}

	dpcm = fopen(dpcm_path, "rb");
	pcm = fopen(pcm_path, "wb");

	index = fgetc(dpcm);
	lp = exponential[index];
	index = fgetc(dpcm);
	rp = exponential[index];

	for (i = 1; !(feof(dpcm)); i++)
	{
		if (ferror(dpcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}

		decompressed_sample = reconstruct_sample(dpcm, lp, rp);
		lp = decompressed_sample->l;
		rp = decompressed_sample->r;
		save_sample(pcm, decompressed_sample);
		free(decompressed_sample);
	}
	fclose(dpcm);
	fclose(pcm);

	return (0);
}
