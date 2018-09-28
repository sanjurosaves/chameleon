#include "../dpcm.h"

int main(void)
{
	FILE *pcm;
	sample *test_sample;

	pcm = fopen("test.pcm", "rb");

	test_sample = get_sample(pcm, 0, 0);

	printf("test_sample->l: %d\n", test_sample->l);
	printf("test_sample->r: %d\n", test_sample->r);
	printf("test_sample->pl: %d\n", test_sample->pl);
	printf("test_sample->pr: %d\n", test_sample->pr);

	fclose(pcm);

	return (0);
}
