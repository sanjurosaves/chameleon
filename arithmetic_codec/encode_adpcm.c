#include "adpcm.h"

#define BUFFER_LEN 20
#define MAX_CHANNELS 2

void adpcm_encode(curr_sample *data, int count, int channels, FILE *outputfile)
{
	int i, chan, cur_sample, prev_sample, delta, predicted_delta;
	unsigned short int sign_bit, code;
	short int index = 0;
	short int two_encoded_samples;
	short int *short_ptr;
	unsigned char byte_l, byte_r;

	for (i = 0; i < count;)
		for (chan = 0; chan < channels; chan++, i++)
		{
			if (i % 2 == 0)
				cur_sample = data[i].l;
			else
				cur_sample = data[i].r;
			if (i < channels)
				prev_sample = 0;
			else
				if (i % 2 == 0)
					prev_sample = data[i - channels].l;
				else
					prev_sample = data[i - channels].r;

			delta = cur_sample - prev_sample;
			if (delta < 0)
			{
				delta = -(delta);
				sign_bit = 1;
			}
			else
				sign_bit = 0;

			code = 4 * delta / ima_step_size[index];
			if (code > 7)
				code = 7;

			index += ima_index_adjust[code];
			if (index < 0)
				index = 0;
			if (index > 88)
				index = 88;

			predicted_delta = (ima_step_size[index] * code) / 4
				+ (ima_step_size[index] / 8);

			prev_sample += predicted_delta;
			if (prev_sample > 32767)
				prev_sample = 32767;
			else if (prev_sample < -32768)
				prev_sample = -32768;

			if (i % 2 == 0)
				byte_l = code + sign_bit * 8;
			else
			{
				byte_r = code + sign_bit * 8;
				two_encoded_samples =
					(((short)byte_l) << 8) | (0x00ff & byte_r);
				short_ptr = &two_encoded_samples;
				printf("2ec: %d\n", two_encoded_samples);
				int wrote = fwrite(short_ptr, 2, 1, outputfile);
				printf("writecount: %d\n", wrote);

			}
		}
}

int main(int argc, char **argv)
{
	FILE *infile, *outfile;
	char* infilename = argv[1];
	char* outfilename = argv[2];
	/* int readcount;*/
	curr_sample **samples;

	if (argc < 3)
	{
		printf("USAGE: encode infile outfile");
		return (-1);
	}

        if (verify_existence(infilename) == -1)
	{
		printf("specified PCM file does not exist");
		return (-1);
	}

	infile = fopen(infilename, "rb");
	if(!infile)
	{
		printf("Not able to open input file");
		return (-1);
	}

	outfile = fopen(outfilename, "wb");
	if(!outfile)
	{
		printf("Not able to open output file");
		return (-1);
	}

	/*while ((readcount = fread(data, 2, BUFFER_LEN, infile)))
	{
		printf("readcount: %d\n", readcount);
		adpcm_encode(data, readcount, 2, outfile);
		}*/

	/*readcount = fread(data, 2, BUFFER_LEN, infile);
	printf("readcount: %d\n", readcount);
	for (int k = 0; k < BUFFER_LEN / 2; k++)
		printf("data[%d]: %d %s", k, data[k], "\n");
		adpcm_encode(data, readcount, 2, outfile);*/

	samples = malloc(sizeof(struct stereo_sample_curr*) * BUFFER_LEN);
	for (int k = 0; k < BUFFER_LEN; k++)
	{
		samples[k] = get_sample(infile);
		printf("l: %d ", samples[k]->l);
		printf("r: %d\n", samples[k]->r);
	}
	adpcm_encode(*samples, 1, 2, outfile);

	for (int k = 0; k < BUFFER_LEN; k++)
		free(samples[k]);
	free(samples);

	fclose(infile);
	fclose(outfile);
}
