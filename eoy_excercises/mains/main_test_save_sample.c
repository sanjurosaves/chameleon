#include "../dpcm.h"

int main(void)
{
	FILE *pcm;
	curr_sample *decompressed_sample;

	pcm = fopen("test.pcm", "wb");

	decompressed_sample = malloc(sizeof(curr_sample));
	if (decompressed_sample == NULL)
		return (-1);

	decompressed_sample->l = -27916;
	decompressed_sample->r = 3053;

	save_sample(pcm, decompressed_sample);

	fclose(pcm);

	return (0);
}
