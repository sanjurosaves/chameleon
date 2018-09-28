#include "../dpcm.h"

int main(void)
{
        quantz_diff *newqdiff;
	FILE *dpcm;

	dpcm = fopen("qdifftest.dpcm", "wb");

	newqdiff = malloc(sizeof(quantz_diff));
	if (newqdiff == NULL)
		return (-1);

	newqdiff->ldiff = 10;
	newqdiff->rdiff = 220;

	save_qdif(dpcm, newqdiff);

	free(newqdiff);

	fclose(dpcm);

	return (0);
}
