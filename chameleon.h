#ifndef CHAMELEON_H_
#define CHAMELEON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sndfile.h>

#define	BUFFER_LEN 4096

/**
 * enum out - enumerates valid output file types
 * @wav:  Microsoft WAV format (little endian) | Signed 16 bit data
 * @wave: Microsoft WAV format (little endian) | Signed 16 bit data
 * @aif:  Apple/SGI AIFF format (big endian)   | Signed 16 bit data
 * @aiff: Apple/SGI AIFF format (big endian)   | Signed 16 bit data
 * @flac: FLAC lossless file format            | Signed 16 bit data
 * @ogg:  Xiph OGG container                   | Xiph Vorbis encoding
 * @ima_adpcm_wav: Microsoft WAV format (little endian) | IMA ADPCM
 */
enum out
{
	wav = 0,
	wave = 0,
	aif = 1,
	aiff = 1,
	flac = 2,
	ogg = 3,
	ima_adpcm_wav = 4,
};

typedef enum out out_t;

static const struct {
	out_t      val;
	const char *str;
} conversion[] = {
	{wav, "wav"},
	{wave, "wave"},
	{aif, "aif"},
	{aiff, "aiff"},
	{flac, "flac"},
	{ogg, "ogg"},
	{ima_adpcm_wav, "ima_adpcm_wav"},
};

static void transcode(
	const char *infilename, const char *outfilename, int filetype);
char *remove_ext_leading_dirs(char *filename);
out_t str2enum(const char *str);

#endif
