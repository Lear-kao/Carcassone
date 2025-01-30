CC=cc
CFLAGS=-Wall -Wextra -Wvla -std=c99 -pedantic -Werror
TESTFLAGS=-lcriterion

all : data_structure.o game_manager_function.o game_manager.o player.o tile.o
	${CC} ${CFLAGS} -o Carcassonne

test :
	${CC} ${TESTFLAGS} ${CFLAGS} -o testSuite