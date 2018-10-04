#include "dpcm.h"
#include "adpcm.h"

int main(int argc, char **argv)
{
	FILE *adpcm, *pcm;
	curr_sample *decompressed_sample, *current_sample;
	unsigned char compressed_sample, l_byte, r_byte;
	unsigned int i, start = 0, end = 0;
	char *pcm_path,	*adpcm_path;
	struct ima_state *state;

	adpcm_path = argv[1];
      	pcm_path = argv[2];

	if (argc < 3)
		printf("USAGE: ");

	if (argc > 3)
	{
		start = atoi(argv[3]);
		end = atoi(argv[4]);
	}

	if (verify_existence(adpcm_path) == -1)
	{
		printf("specified DPCM file does not exist\n");
		return (-1);
	}

	adpcm = fopen(adpcm_path, "rb");
	pcm = fopen(pcm_path, "wb");

	/*cheating with initial sample value
	state->plv = -513;
	state->prv = -257;
	end cheating*/

	for (i = 1; !(feof(adpcm)); i++)
	{
		if (ferror(adpcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}
		if (i == 1)
		{
			current_sample = get_sample(adpcm);
			if (current_sample == NULL)
				break;
			save_sample(pcm, current_sample);

			state = malloc(sizeof(struct ima_state));
			if (state == NULL)
				return (-1);
			state->li = atoi(argv[3]);
			state->li = atoi(argv[4]);
			state->plv = current_sample->l;
			state->prv = current_sample->r;

			free(current_sample);
		}
		else
		{

			decompressed_sample = malloc(sizeof(curr_sample));
			if (decompressed_sample == NULL)
				return (-1);
			decompressed_sample->l = 0;
			decompressed_sample->r = 0;

			compressed_sample = fgetc(adpcm);
			if (feof(adpcm))
			{
				free(decompressed_sample);
				break;
			}

			l_byte = compressed_sample >> 4;
			r_byte = compressed_sample & 0x000f;

			decompressed_sample->l = decompress_adpcm_to_pcm(&state->li, &state->plv, l_byte);
			decompressed_sample->r = decompress_adpcm_to_pcm(&state->ri, &state->prv, r_byte);
			if ((i > start) && (i < end))
			{
				printf("l: %d r: %d\n", decompressed_sample->l, decompressed_sample->r);
				printf("delta_code_l: %d delta_code_r: %d\n", l_byte, r_byte);
			}

			save_sample(pcm, decompressed_sample);
			free(decompressed_sample);
		}
	}
	fclose(adpcm);
	fclose(pcm);
	free(state);

	return (0);
}
