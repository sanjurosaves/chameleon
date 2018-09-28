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

int verify_existance(char *path)
{
	if (access(path, F_OK) == -1)
		return (-1);

	return (0);
}
