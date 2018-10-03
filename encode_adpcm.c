#include "adpcm.h"

#define BUFFER_LEN 5909500
#define MAX_CHANNELS 2

void adpcm_encode(int *data, int count, int channels, SNDFILE *outputfile, FILE *ofile)
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
			cur_sample = data[i];
			if (i < channels)
				prev_sample = 0;
			else
				prev_sample = data[i - channels];

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

			byte_l = code + sign_bit * 8;

			fputc(byte_l, ofile);

			if (i % 2 == 0)
				byte_l = code + sign_bit * 8;
			else
			{
				byte_r = code + sign_bit * 8;
				two_encoded_samples =
					(((short)byte_l) << 8) | (0x00ff & byte_r);
				short_ptr = &two_encoded_samples;
				sf_writef_short(outputfile, short_ptr, 1);
			}
		}
}

int main(int argc, char **argv)
{
	SNDFILE *infile, *outfile;
	char* infilename = argv[1];
	char* outfilename = argv[2];
	SF_INFO sfinfo;
	int readcount;
	static int data[BUFFER_LEN];
	FILE *ofile;


	memset(&sfinfo, 0, sizeof(sfinfo));

	if (argc < 3)
	{
		printf("USAGE: encode infile outfile");
		return (-1);
	}

	/* open infile (SFM_READ is mode of sfopen)*/
	infile = sf_open(infilename, SFM_READ, &sfinfo);
	if(!infile)
	{
		printf("Not able to open input file");
		puts(sf_strerror(NULL));
		return (1);
	}

	sfinfo.format = 0x010000 | 0x0012;
	/* open output file */
	outfile = sf_open(outfilename, SFM_WRITE, &sfinfo);
	if(!outfile)
	{
		printf("Not able to open output file");
		puts(sf_strerror(NULL));
		return (1);
	}

	/* open self-built output file */
	ofile = fopen("ofiledpcm.wav", "wb");

	if (sfinfo.channels > MAX_CHANNELS)
	{
		printf("Input file contains too many channels.");
		return (-1);
	}

	printf("Format: %d\n", sfinfo.format);
	printf("Channels: %d\n", sfinfo.channels);
	printf("Samplerate: %d\n", sfinfo.samplerate);
	printf("Sections: %d\n", sfinfo.sections);

	while ((readcount = sf_read_int(infile, data, BUFFER_LEN)))
	{
		adpcm_encode(data, readcount, sfinfo.channels, outfile, ofile);
	}

	fclose(ofile);
	sf_close(infile);
	sf_close(outfile);
}
