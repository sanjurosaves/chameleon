#ifndef DPCM_H_
#define DPCM_H_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * struct stereo_sample - structure for 16-bit stereo sample
 * @l: value of left-channel sample
 * @r: value of right-channel sample
 * @pl: value of previous left-channel sample
 * @pr: value of previous right-channel sample
 */
struct stereo_sample
{
	int l, r, pl, pr;
};

/**
 * struct stereo_sample_diff - structure to hold differentials b/w samples
 * @ldiff: difference b/w values of left-channel samples
 * @rdiff: difference b/w values of right-channel samples
 * @l_s_b: left sign bit
 * @r_s_b: right sign bit
 */
struct stereo_sample_diff
{
	int ldiff, rdiff;
	unsigned short int l_s_b, r_s_b;
};

/**
 * struct stereo_quantized_diff - struct to hold quantized differentials
 * @byte_l: quantized difference b/w values of left-channel samples
 * @byte_r: quantized difference b/w values of right-channel samples
 */
struct stereo_quantized_diff
{
	unsigned char byte_l, byte_r;
};

/**
 * struct stereo_sample_curr - structure for 16-bit stereo sample
 * @l: value of left-channel sample
 * @r: value of right-channel sample
 */
struct stereo_sample_curr
{
	int l, r;
};


typedef struct stereo_sample sample;
typedef struct stereo_sample_diff sample_diff;
typedef struct stereo_quantized_diff quantz_diff;
typedef struct stereo_sample_curr curr_sample;

/* encoding and decoding */
int compress_pcm_to_adpcm(int sample, int *index, int *prev);
int decompress_adpcm_to_pcm(int *index, int *prev, int delta_code);

/* sample handling */
curr_sample *get_sample(FILE *pcm);
void save_sample(FILE *pcm, curr_sample *decompressed_sample);
curr_sample *reconstruct_sample(FILE *adpcm);
curr_sample *convert_sample_to_curr_sample(sample *current_sample);

/* delta handling */
sample_diff *get_diff_stereo(curr_sample *current_sample, int lp, int rp);
quantz_diff *quantize_diff_stereo(sample_diff *diff, int lp, int rp);
int save_qdif(FILE *dpcm, quantz_diff *qdif);

/* utilities */
unsigned char bsearcharray(int diff, unsigned char min, unsigned char max);
int verify_existence(char *path);

#endif
