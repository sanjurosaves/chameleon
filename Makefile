CC = gcc
ENCODESRC=mains/main_compress_pcm_to_dpcm.c dpcm.c deltas.c samples.c util.c
DECODESRC=mains/main_decompress_dpcm.c dpcm.c deltas.c samples.c util.c
CFLAGS=-Wall -Werror -Wextra -pedantic

all:
	$(CC) $(CFLAGS) $(ENCODESRC) -o encode
	$(CC) $(CFLAGS) $(DECODESRC) -o decode
