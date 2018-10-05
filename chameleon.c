#include "chameleon.h"

/**
 * main - accepts command line arguments and calls transcoder accordingly
 * @argc: count of arguments
 * @argv: cli arguments
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char **argv)
{
	out_t selected_type;
	char *base;

	if (argc < 3)
	{
		printf("\tUsage : chameleon <inputfile> output_type\n");
		printf("\t\tValid output types:\n");
		printf("\t\t\t\t\togg\n");
		printf("\t\t\t\t\tflac\n");
		printf("\t\t\t\t\twave\n");
		printf("\t\t\t\t\taiff\n");
		printf("\t\t\t\t\tima_adpcm_wav\n");
		return (-1);
	}

	base = remove_ext_leading_dirs(argv[1]);

	selected_type = str2enum(argv[2]);

	if (selected_type == 0)
		transcode(argv[1], strcat(base, ".wav"),
			  SF_FORMAT_WAV | SF_FORMAT_PCM_16);
	if (selected_type == 1)
		transcode(argv[1], strcat(base, ".aiff"),
			  SF_FORMAT_AIFF | SF_FORMAT_PCM_16);
	if (selected_type == 2)
		transcode(argv[1], strcat(base, ".flac"),
			  SF_FORMAT_FLAC | SF_FORMAT_PCM_16);
	if (selected_type == 3)
		transcode(argv[1], strcat(base, ".ogg"),
			  SF_FORMAT_OGG | SF_FORMAT_VORBIS);
	if (selected_type == 4)
		transcode(argv[1], strcat(base, "_ima_adpcm.wav"),
			  SF_FORMAT_WAV | SF_FORMAT_IMA_ADPCM);

	return (0);
}


/**
 * transcode - transcodes audio file from one format/encoding to another
 * @infilename: input file
 * @outfilename: output file
 * @filetype: user specified type of output file
 */
static void transcode(const char *infilename,
		      const char *outfilename, int filetype)
{
	static float buffer[BUFFER_LEN];

	SNDFILE *infile, *outfile;
	SF_INFO	sfinfo;
	int readcount;

	printf("    %s -> %s ", infilename, outfilename);
	fflush(stdout);

	memset(&sfinfo, 0, sizeof(sfinfo));

	printf("... ");

	infile = sf_open(infilename, SFM_READ, &sfinfo);
	if (!infile)
	{
		printf("Error : could not open file : %s\n", infilename);
		puts(sf_strerror(NULL));
		exit(1);
	}

	sfinfo.format = filetype;

	if (!sf_format_check(&sfinfo))
	{
		sf_close(infile);
		printf("Invalid encoding\n");
		return;
	};

	outfile = sf_open(outfilename, SFM_WRITE, &sfinfo);
	if (!outfile)
	{
		printf("Error : could not open file : %s\n", outfilename);
		puts(sf_strerror(NULL));
		exit(1);
	}

	while ((readcount = sf_read_float (infile, buffer, BUFFER_LEN)) > 0)
		sf_write_float (outfile, buffer, readcount);

	sf_close(infile);
	sf_close(outfile);

	printf("ok\n");
/*
** In accordance with the permitted use the libsndfile API, from which this
** function derives, the following notice is included in this source code.
**
** Copyright (C) 2002-2011 Erik de Castro Lopo <erikd@mega-nerd.com>
**
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in
**       the documentation and/or other materials provided with the
**       distribution.
**     * Neither the author nor the names of any contributors may be used
**       to endorse or promote products derived from this software without
**       specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
** OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
** ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
}

/**
 * remove_ext_leading_dirs - removes extension and leading
 * directories from original file name path
 * @filename: original file name path to be trimmed
 * Return: pointer to trimmed basename
 */
char *remove_ext_leading_dirs(char *filename)
{
	static char basename[99];
	char d = '.';
	int i, j;

	for (i = strlen(filename); i > 0; i--)
	{
		if (filename[i] == d)
			break;
	}

	for (j = 0; j < i && filename[j] != '\0'; j++)
		basename[j] = filename[j];

	basename[j + 1] = '\0';

	d = '/';

	while (1)
	{
		for (i = 0; basename[i] != '\0'; i++)
		{
			if (basename[i] == d)
				break;
			if (basename[i + 1] == '\0')
				return (basename);
		}

		i++;

		for (j = 0; basename[j] != '\0'; j++)
		{
			basename[j] = basename[i + j];
		}
	}
	return (basename);
}

/**
 * str2enum - converts string to enum
 * @str: string to convert
 * Return: string converted to enum
 */
out_t str2enum(const char *str)
{
	unsigned int j;

	for (j = 0;  j < sizeof(conversion) / sizeof(conversion[0]);  ++j)
		if (!strcmp(str, conversion[j].str))
			return (conversion[j].val);

	printf("Not a valid output type.\n");
	printf("\t\tValid output types:\n");
	printf("\t\t\t\t\togg\n");
	printf("\t\t\t\t\tflac\n");
	printf("\t\t\t\t\twave\n");
	printf("\t\t\t\t\taiff\n");
	printf("\t\t\t\t\tima_adpcm_wav\n");
	return (conversion[j].val);
}
