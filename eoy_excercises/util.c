#include "dpcm.h"

unsigned char bsearcharray(int diff, unsigned char min,
			   unsigned char max)
{
	int disttomid, disttomidoffset;
	int mid = ((max - min) / 2) + min;

	if ((diff < -65535) || (diff > 65535))
	{
		printf("error: sample value out of range");
		return (-0);
	}
	if (diff >= exponential[max])
		return (max);
	if (diff < exponential[min])
		return (min);

	if (diff > exponential[mid])
	{
		if (diff < exponential[(mid) + 1])
		{
			disttomid = abs(diff - exponential[mid]);
			disttomidoffset = abs(diff - exponential[(mid) + 1]);
			return (disttomid <= disttomidoffset ? mid : mid + 1);
		}
		else
		{
			min = mid;
		}
	}
	else if (diff < exponential[mid])
	{
		if (diff > exponential[(mid) - 1])
		{
			disttomid = abs(diff - exponential[mid]);
			disttomidoffset = abs(diff - exponential[(mid) - 1]);
			return (disttomid <= disttomidoffset ? mid : mid - 1);
		}
		else
		{
			max = mid;
		}
	}
	if (diff == exponential[mid])
		return (mid);

	return(bsearcharray(diff, min, max));
}
/*
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
*/
