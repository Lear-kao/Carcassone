CC=gcc
PROG = program
CFLAGS=-std=c99 -pedantic
GDBFLAGS=-g
TESTFLAGS=-lcriterion
INC_Dir = header
SRC_Dir= program
OBJ_Dir = oFile
all = exe

default = $(all)


all: $(OBJ_Dir)/data_structure.o $(OBJ_Dir)/game_manager.o $(OBJ_Dir)/game_manager_function.o $(OBJ_Dir)/player.o $(OBJ_Dir)/tile.o $(OBJ_Dir)/test.o $(OBJ_Dir)/func_point.o

$(OBJ_Dir)/data_structure.o : $(SRC_Dir)/data_structure.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/data_structure.c $(CFLAGS) -o $(OBJ_Dir)/data_structure.o

$(OBJ_Dir)/game_manager_function.o : $(SRC_Dir)/game_manager_function.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/game_manager_function.c $(CFLAGS) -o $(OBJ_Dir)/game_manager_function.o

$(OBJ_Dir)/game_manager.o : $(SRC_Dir)/game_manager.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/game_manager.c $(CFLAGS) -o $(OBJ_Dir)/game_manager.o

$(OBJ_Dir)/player.o : $(SRC_Dir)/player.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/player.c $(CFLAGS) -o $(OBJ_Dir)/player.o

$(OBJ_Dir)/tile.o : $(SRC_Dir)/tile.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/tile.c $(CFLAGS) -o $(OBJ_Dir)/tile.o

$(OBJ_Dir)/func_point.o : $(SRC_Dir)/func_point.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/func_point.c $(CFLAGS) -o $(OBJ_Dir)/func_point.o

$(OBJ_Dir)/test.o : $(SRC_Dir)/testSuite.c $(INC_Dir)/Carcassonne.h
	$(CC) -c $(SRC_Dir)/testSuite.c $(CFLAGS) -o $(OBJ_Dir)/test.o

test : $(OBJ_Dir)/test.o
	${CC} ${CFLAGS} ${TESTFLAGS} $(OBJ_Dir)/test.o -o testSuite
	./testSuite

clear : 
	rm testSuite $(OBJ_Dir)/test.o $(OBJ_Dir)/data_structure.o $(OBJ_Dir)/func_point.o $(OBJ_Dir)/game_manager.o $(OBJ_Dir)/game_manager_function.o $(OBJ_Dir)/player.o $(OBJ_Dir)/tile.o 
