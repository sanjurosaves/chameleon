#include "adpcm.h"

#define BUFFER_LEN 4
#define MAX_CHANNELS 2

void adpcm_decode(short int *data, int count, int channels, SNDFILE *outputfile)
{
	int i, chan, cur_sample = 0, delta, written;
	unsigned short int sign_bit;
	short int code, index = 0;
	unsigned char l_byte, r_byte;
	int int_ptr[2];

	printf("Count: %d\n", count);
	for (i = 0; i < count;)
		for (chan = 0; chan < channels; chan++, i++)
		{
			if (i < 21)
				printf("data[i]: %d\n", data[i]);
			if (i % channels == 0)
			{
				l_byte = data[i] >> 8;
				r_byte = data[i] & 0x00ff;
				code = l_byte;
			}
			else
				code = r_byte;

			/* if (i < 21)
			   printf("pre-process code: %d\n", code); */
			/* --separate sign bit from other bits-- */
			if (code & (1 << 7))
				sign_bit = 1;
			else
				sign_bit = 0;
			code = ~(1 << 7) & code;
			/*if ((400000 < i) && (i < 400044))
			  printf("code: %d\ndelta: %d\n", code, delta);*/

			delta = ((ima_step_size[index] * code) / 4)
				+ (ima_step_size[index] / 8);

			if (sign_bit == 1)
				delta = -(delta);

			cur_sample = cur_sample + delta;
			if (cur_sample > 32767)
				cur_sample = 32767;
			else if (cur_sample < -32768)
				cur_sample = -32768;

			if (i % channels == 0)
				int_ptr[0] = cur_sample;
			else
			{
				int_ptr[1] = cur_sample;
				/* if ((400000 < i) && (i < 400044))
				   printf("cur_samplel: %d\n", int_ptr[0]); */
				written = sf_write_int(outputfile, int_ptr, 2);
				if (written == 0)
				{
					printf("written: %d\n", written);
					return;
				}
			}
			index += ima_index_adjust[code];
			if (index < 0)
				index = 0;
			if (index > 88)
				index = 88;

	}
	printf("i: %d\n", i);
}

int main(int argc, char **argv)
{
	SNDFILE *infile, *outfile;
	char* infilename = argv[1];
	char* outfilename = argv[2];
	SF_INFO sfinfo;
	int readcount;
	short int data[BUFFER_LEN];

	memset(&sfinfo, 0, sizeof(sfinfo));

	if (argc < 3)
	{
		printf("USAGE: decode adpcm_infile wav_outfile\n");
		return (-1);
	}

	/* open infile (SFM_READ is mode of sfopen)*/
	infile = sf_open(infilename, SFM_READ, &sfinfo);
	if(!infile)
	{
		printf("Not able to open input file\n");
		puts(sf_strerror(NULL));
		return (1);
	}

	printf("Format: %d\n", sfinfo.format);
	sfinfo.format = 0x010000 | 0x0002;
	/* open output file */
	outfile = sf_open(outfilename, SFM_WRITE, &sfinfo);
	if(!outfile)
	{
		printf("Not able to open output file\n");
		puts(sf_strerror(NULL));
		return (1);
	}

	if (sfinfo.channels > MAX_CHANNELS)
	{
		printf("Input file contains too many channels.\n");
		return (-1);
	}

	printf("Format: %d\n", sfinfo.format);
	printf("Channels: %d\n", sfinfo.channels);
	printf("Samplerate: %d\n", sfinfo.samplerate);
	printf("Sections: %d\n", sfinfo.sections);

	readcount = sf_read_raw(infile, data, BUFFER_LEN);
	printf("readcount: %d\n", readcount);
	for (int i = 0; i < BUFFER_LEN; i++)
		printf("data[i]: %d\n", data[i]);
/*		adpcm_decode(data, readcount, sfinfo.channels, outfile);*/
/*		getchar();
		printf("data: %d\n", data[0]); */

	sf_close(infile);
	sf_close(outfile);
}
