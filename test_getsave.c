#include "dpcm.h"
#include "adpcm.h"

int main(int argc, char **argv)
{
	char *pcm_path, *adpcm_path;
	FILE *pcm, *adpcm;
	unsigned int i = 1;
	curr_sample *current_sample;

	if (argc != 3)
		printf("USAGE!");
	
	pcm_path = argv[1];
	adpcm_path = argv[2];

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
		else
		{
			current_sample = get_sample(pcm);
			if (current_sample == NULL)
				break;
			save_sample(adpcm, current_sample);
			free(current_sample);
		}
	}
	printf("i: %d\n", i);
	fclose(pcm);
	fclose(adpcm);

	return (0);

}
