#include "../dpcm.h"

int main(void)
{
	FILE *pcm;
	sample *test_sample;
	sample_diff *test_diff;

	pcm = fopen("test.pcm", "rb");

	test_sample = get_sample(pcm, 0, 0);

	printf("test_sample->l: %d\n", test_sample->l);
	printf("test_sample->r: %d\n", test_sample->r);
	printf("test_sample->pl: %d\n", test_sample->pl);
	printf("test_sample->pr: %d\n", test_sample->pr);

	test_diff = get_diff_stereo(test_sample);

	printf("test_diff->ldiff: %d\n", test_diff->ldiff);
	printf("test_diff->ldiff: %d\n", test_diff->rdiff);


	fclose(pcm);
	free(test_sample);
	free(test_diff);

	return (0);
}
