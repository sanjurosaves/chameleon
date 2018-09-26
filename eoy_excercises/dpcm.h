#ifndef DPCM_H
#define DPCM_H

/**
 * struct stereo_sample - structure for 16-bit stereo sample
 * @l: value of left-channel sample
 * @r: value of right-channel sample
 * @pl: value of previous left-channel sample
 * @pr: value of previous right-channel sample
 */
struct stereo_sample {
	int l, r, pl, pr;
};

typedef struct stereo_sample sample;
#endif
