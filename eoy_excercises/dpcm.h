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

/**
 * struct stereo_sample_diff - structure to hold differentials b/w samples
 * @ldiff: difference b/w values of left-channel samples
 * @rdiff: difference b/w values of right-channel samples
 */
struct stereo_sample_diff {
	int ldiff, rdiff;
};

typedef struct stereo_sample sample;
typedef struct stereo_sample_diff sample_diff;

#endif
