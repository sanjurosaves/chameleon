#ifndef DPCM_H
#define DPCM_H

/**
 * struct stereo_sample - structure for 16-bit stereo sample
 * @hi: high-order (leftmost) byte of 16-bit sample
 * @lo: low-order (rightmost) byte of 16-bit sample
 * @l: value of left-channel sample
 * @r: value of right-channel sample
 * @pl: value of previous left-channel sample
 * @pr: value of previous right-channel sample
 * @shifted: high-order byte expanded to 16-bit value and shifted left 8 bits
 */
struct stereo_sample {
	char hi, lo;
	int l, r, pl, pr;
	short int shifted;
};

typedef struct stereo_sample sample;
#endif
