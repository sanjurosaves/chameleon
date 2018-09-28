#include "../dpcm.h"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("USAGE: decompress dpcm_file pcm_file\n");
		return (-1);
	}

	return (decompress_dpcm_to_pcm(argv[1], argv[2]));
}
