#include "dpcm.h"
#include "adpcm.h"

/**
 * compress_pcm_to_adpcm - encodes pcm file to adpcm
 * @pcm_path: path to pcm file
 * @adpcm_path: path adpcm file
 * Return: 0 on success, -1 on failure
 */
int compress_pcm_to_adpcm(int sample, int *index, int *prev)
{
	int diff = sample - *prev;
	int step = ima_step_size[*index];
	int delta_code = 0;

	if (diff < 0)
	{
		delta_code = 8;
		diff = -(diff);
	}

	if (diff >= step)
	{
		delta_code |= 4;
		diff -= step;
	}
	step >>= 1;
	if (diff >= step)
	{
		delta_code |= 2;
		diff -= step;
	}
	step >>= 1;
	if (diff >= step)
	{
		delta_code |= 1;
		diff -= step;
	}

	decompress_adpcm_to_pcm(*&index, *&prev, delta_code);
	return (unsigned char)delta_code;
}
