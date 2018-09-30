#include "../dpcm.h"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("USAGE: compress pcm_file dpcm_file\n");
		return (-1);
	}

	return (compress_pcm_to_dpcm(argv[1], argv[2]));
}
