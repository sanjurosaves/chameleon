#include "dpcm.h"
#include "adpcm.h"

/**
 * decompress_adpcm_to_pcm - decodes adpcm file to pcm
 * @pcm_path: path to pcm file
 * @adpcm_path: path adpcm file
 * Return: 0 on success, -1 on failure
 */
int decompress_adpcm_to_pcm(int *index, int *prev, int delta_code)
{
	int step = ima_step_size[*index], difference;

	difference = step >> 3;
	if (delta_code & 1)
		difference += step >> 2;
	if (delta_code & 2)
		difference += step >> 1;
	if (delta_code & 4)
		difference += step;
	if (delta_code & 8)
		difference = -(difference);

	*prev += difference;
	if (*prev > 32767)
		*prev = 32767;
	else if (*prev < -32768)
		*prev = -32768;

	*index += ima_index_adjust[delta_code];
	if (*index < 0)
		*index = 0;
	else if (*index > 88)
		*index = 88;

	return *prev;
}
