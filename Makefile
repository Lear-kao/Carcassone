CC=cc
CFLAGS=-std=c99 -pedantic
TESTFLAGS=-lcriterion

all : data_structure.o game_manager_function.o game_manager.o player.o tile.o
	${CC} ${CFLAGS} -o Carcassonne

test :
	${CC} ${CFLAGS} ${TESTFLAGS} testSuite.c -o testSuite
	./testSuite
clear : 
	rm a.out data_structure.o game_manager_function.o game_manager.o player.o tile.o testSuite
	