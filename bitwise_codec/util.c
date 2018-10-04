#include "dpcm.h"

/**
 * verify_existence - confirm objective reality of file
 * @path: path of supposed file
 * Return: -1 if file doesn't exist, 0 if it does exist
 */
int verify_existence(char *path)
{
	if (access(path, F_OK) == -1)
		return (-1);

	return (0);
}
