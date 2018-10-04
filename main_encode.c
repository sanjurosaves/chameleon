#include "dpcm.h"
#include "adpcm.h"

int main(int argc, char **argv)
{
	char *pcm_path, *adpcm_path;
	FILE *pcm, *adpcm;
	unsigned int i = 1, start, end;
	curr_sample *current_sample;
	quantz_diff *qdif;
	int status = 0;
	struct ima_state *state;

	pcm_path = argv[1];
	adpcm_path = argv[2];

	if (argc < 3)
		printf("USAGE: ");
	if (argc > 3)
	{
		start = atoi(argv[3]);
		end = atoi(argv[4]);
	}

	if (verify_existence(pcm_path) == -1)
	{
		printf("specified PCM file does not exist");
		return (-1);
	}

	pcm = fopen(pcm_path, "rb");
	adpcm = fopen(adpcm_path, "wb");

	/* cheating with real initial value
	state->plv = -513;
	state->prv = -257;
	get_sample(pcm);
	end cheating */

	qdif = malloc(sizeof(quantz_diff));
	if (qdif == NULL)
		return (-1);

	for (i = 1; !(feof(pcm)); i++)
	{
		if (ferror(pcm))
		{
			int ec = errno;

			printf("ERROR AT SAMPLE #: %d\n", i);
			printf("ERROR CODE: %d\n", ec);
			return (-1);
		}
		if (i == 1)
		{
			current_sample = get_sample(pcm);
			if (current_sample == NULL)
				break;
			save_sample(adpcm, current_sample);

			state = malloc(sizeof(struct ima_state));
			if (state == NULL)
				return (-1);
			state->li = atoi(argv[3]);
			state->ri = atoi(argv[4]);
			state->plv = current_sample->l;
			state->prv = current_sample->r;
			free(current_sample);
		}
		else
		{
			current_sample = get_sample(pcm);
			if (current_sample == NULL)
				break;
/*			printf("l: %d, r: %d\n", current_sample->l, current_sample->r);*/

			qdif->byte_l = compress_pcm_to_adpcm(current_sample->l, &state->li, &state->plv);
			qdif->byte_r = compress_pcm_to_adpcm(current_sample->r, &state->ri, &state->prv);

			if ((i > start) && (i < end))
			{
				printf("l: %d, r: %d\n", current_sample->l, current_sample->r);
				printf("delta_code_l: %d ", qdif->byte_l);
				printf("delta_code_r: %d\n ", qdif->byte_r);
			};
			status = save_qdif(adpcm, qdif);
			if (status == -1)
				return (-1);

			free(current_sample);
		}
	}
	printf("i: %d\n", i);
	fclose(pcm);
	fclose(adpcm);
	free(state);
	free(qdif);

	return (0);

}
