CC=cc
CFLAGS=-Wall -Wextra -Wvla -std=c99 -pedentic -Werror

all : initBorder.o initTuile.o
	${CC} ${CFLAGS} -o Carcassonne