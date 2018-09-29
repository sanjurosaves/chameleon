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

	printf("min: %d , max: %d, mid: %d, compval: %d, diff: %d\n", min, max, mid, exponential[mid], diff);
	getchar();

	if (diff > exponential[mid])
	{
		printf("if outer\n");
		if (diff < exponential[(mid) + 1])
		{
			printf("if inner\n");
			disttomid = abs(diff - exponential[mid]);
			disttomidoffset = abs(diff - exponential[(mid) + 1]);
			return (disttomid <= disttomidoffset ? mid : mid + 1);
		}
		else
		{
			printf("if outer else\n");
			min = mid;
		}
	}
	else if (diff < exponential[mid])
	{
		printf("else if outer\n");
		if (diff > exponential[(mid) - 1])
		{
			printf("else if inner if\n");
			disttomid = abs(diff - exponential[mid]);
			disttomidoffset = abs(diff - exponential[(mid) - 1]);
			return (disttomid <= disttomidoffset ? mid : mid + 1);
		}
		else
		{
			printf("else if outer else\n");
			max = mid;
		}
	}
	if (diff == exponential[mid])
		return (mid);

	return(bsearcharray(diff, min, max));
}

int main(void)
{
	sample_diff *diff;
	unsigned char index;
	int i = 0;

	diff = malloc(sizeof(sample_diff));
	if (diff == NULL)
		return (-1);

	/* scanf("%d", &diff->ldiff);*/

	for(i = 0; i < 256; i++)
	{
		diff->ldiff = exponential[i];
		index = bsearcharray(diff->ldiff, 0, 255);
		printf("%d\n", index);
	}

	free(diff);

	return (0);
}
