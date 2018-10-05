CC = gcc
SRC=chameleon.c
CFLAGS=-lsndfile -Wall -Werror -Wextra -pedantic

all:
	$(CC) $(SRC) $(CFLAGS) -o chameleon
