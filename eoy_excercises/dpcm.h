#ifndef DPCM_H_
#define DPCM_H_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static const int exponential[256] = {
	-60226, -58368, -55552, -54272, -52992, -51712, -50432, -49152, -47872,
	-46592, -45312, -44032, -42752, -41472, -40192, -38912, -37632, -36352,
	-35073, -33537, -32769, -30209, -29697, -29185, -27649, -27393, -27136,
	-26881, -26625, -26369, -26113, -25857, -25600, -25344, -25088, -24832,
	-24576, -24321, -24064, -23808, -23552, -23296, -23040, -22785, -22529,
	-22272, -22017, -21760, -21504, -21249, -20992, -20736, -20480, -20224,
	-19968, -19712, -19456, -19200, -18944, -18689, -18432, -18176, -17921,
	-17665, -17408, -17152, -16896, -16641, -16384, -16129, -15872, -15616,
	-15360, -15105, -14848, -14592, -14336, -14081, -13824, -13568, -13312,
	-13056, -12800, -12544, -12288, -12032, -11776, -11521, -11264, -11008,
	-10752, -10496, -10240, -9984, -9729, -9472, -9216, -8960, -8704, -8448,
	-8192, -7936, -7680, -7424, -7168, -6912, -6656, -6400, -6144, -5888,
	-5632, -5376, -5120, -4864, -4608, -4352, -4096, -3840, -3584, -3328,
	-3072, -2816, -2561, -2305, -2049, -1793, -1537, -1281, -1025, -769,
	-513, -257, 0, 256, 512, 768, 1024, 1280, 1536, 1792, 2048, 2304, 2560,
	2816, 3072, 3328, 3584, 3840, 4096, 4352, 4608, 4864, 5120, 5376, 5632,
	5888, 6144, 6400, 6656, 6912, 7168, 7424, 7680, 7936, 8192, 8448, 8704,
	8960, 9216, 9472, 9728, 9984, 10240, 10496, 10752, 11008, 11264, 11520,
	11776, 12032, 12288, 12544, 12800, 13056, 13312, 13568, 13824, 14080,
	14336, 14592, 14848, 15104, 15360, 15616, 15872, 16128, 16384, 16640,
	16896, 17152, 17408, 17664, 17920, 18176, 18432, 18688, 18944, 19200,
	19456, 19712, 19968, 20224, 20480, 20736, 20992, 21248, 21504, 21760,
	22016, 22272, 22528, 22784, 23040, 23296, 23552, 23808, 24064, 24320,
	24576, 24832, 25600, 25856, 27392, 28672, 28928, 32512, 33537, 35073,
	36352, 37632, 38912, 40192, 41472, 42752, 44032, 45312, 46592, 47872,
	49152, 50432, 51712, 52992, 54272, 55552, 58368, 60226 };

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
curr_sample *convert_sample_to_curr_sample(sample *current_sample);

#endif
