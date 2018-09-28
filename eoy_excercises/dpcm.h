#ifndef DPCM_H_
#define DPCM_H_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static const int exponential[256] = {
	-65535, -60227, -55349, -50866, -46746, -42959, -39480, -36282, -33343,
	-30642, -28160, -25879, -23783, -21857, -20086, -18459, -16964, -15590,
	-14327, -13167, -12100, -11120, -10220, -9392, -8631, -7932, -7290,
	-6699, -6157, -5658, -5200, -4779, -4392, -4036, -3709, -3409, -3133,
	-2879, -2646, -2431, -2235, -2054, -1887, -1734, -1594, -1465, -1346,
	-1237, -1137, -1045, -960, -883, -811, -746, -685, -630, -579, -532,
	-489, -449, -413, -380, -349, -321, -295, -271, -249, -229, -210, -193,
	-178, -163, -150, -138, -127, -117, -107, -99, -91, -83, -77, -70, -65,
	-60, -55, -50, -46, -43, -39, -36, -33, -31, -28, -26, -24, -22, -20,
	-19, -17, -16, -15, -13, -12, -11, -11, -10, -9, -8, -8, -7, -7, -6, -6,
	-5, -5, -4, -4, -4, -4, -3, -3, -3, -3, -3, -2, -2, -2, -2, 0, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 9, 10, 10, 11,
	12, 14, 15, 16, 18, 19, 21, 23, 25, 27, 30, 32, 35, 38, 42, 45, 49, 54,
	59, 64, 69, 76, 82, 90, 98, 106, 116, 126, 137, 149, 162, 177, 192, 209,
	228, 248, 270, 294, 320, 348, 379, 412, 448, 488, 531, 578, 629, 684,
	745, 810, 882, 959, 1044, 1136, 1236, 1345, 1464, 1593, 1733, 1886,
	2053, 2234, 2430, 2645, 2878, 3132, 3408, 3708, 4035, 4391, 4778, 5199,
	5657, 6156, 6698, 7289, 7931, 8630, 9391, 10219, 11119, 12099, 13166,
	14326, 15589, 16963, 18458, 20085, 21856, 23782, 25878, 28159, 30641,
	33342, 36281, 39479, 42958, 46745, 50865, 55348, 60226 };

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
 */
struct stereo_sample_diff
{
	int ldiff, rdiff;
};

/**
 * struct stereo_quantized_diff - struct to hold quantized differentials
 * @ldiff: quantized difference b/w values of left-channel samples
 * @rdiff: quantized difference b/w values of right-channel samples
 */
struct stereo_quantized_diff
{
	short int ldiff, rdiff;
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

unsigned char bsearcharray(int diff, unsigned char min, unsigned char max);
sample *get_sample(FILE *pcm, int lp, int rp);
sample_diff *get_diff_stereo(sample *current_sample);
quantz_diff *quantize_diff_stereo(sample_diff *diff);
curr_sample *reconstruct_sample(FILE *dpcm, int lp, int rp);
int save_qdif(FILE *dpcm, quantz_diff *qdif);
int compress_pcm_to_dpcm(char *pcm_path, char *dpcm_path);
int decompress_dpcm_to_pcm(char *dpcm_path, char *pcm_path);
void save_sample(FILE *pcm, curr_sample *decompressed_sample);
int verify_existance(char *path);

#endif
