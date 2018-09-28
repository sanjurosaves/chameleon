#include "../dpcm.h"

int main(void)
{
	FILE *dpcm;
	curr_sample *rs;

	dpcm = fopen("qdifftest.dpcm", "rb");

	rs = reconstruct_sample(dpcm, 0, 0);

	printf("rs->l: %d\n", rs->l);
	printf("rs->r: %d\n", rs->r);

	free(rs);

	return (0);
}
