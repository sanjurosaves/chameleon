#include "dpcm.h"

int main(void)
{
	sample_diff *diff;
	unsigned char index;
	int i = 0;

	diff = malloc(sizeof(sample_diff));
	if (diff == NULL)
		return (-1);

	scanf("%d", &diff->ldiff);
	index = bsearcharray(diff->ldiff, 0, 255);
	printf("%d\n", index);

	for(i = 0; i < 256; i++)
	{
		diff->ldiff = exponential[i] + 500;
		index = bsearcharray(diff->ldiff, 0, 255);
		printf("returned index: %d\n", index);
		printf("quantized value: %d\n", exponential[index]);
		printf("original value: %d\n", exponential[i] + 500);

		if ((exponential[i] + 500) - exponential[index] >
		    (exponential[i] + 500) - exponential[index - 1])
			printf("top fail");
		if ((exponential[i] + 500) - exponential[index] <
		    (exponential[i] + 500) - exponential[index + 1])
			printf("bot fail");
	}
	free(diff);

	return (0);
}
