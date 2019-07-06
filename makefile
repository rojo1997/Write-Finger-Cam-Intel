# Directorios
INCLUDE=./include
LIB=lib
OBJ=obj
SRC=src
BIN=bin
CC=g++-8

# Opciones del compilador
OPT=-O2 -std=c++17 -W -Wall

# REALSENSE
RS2=-lrealsense2

all: clean main

main: main.o matrix.o point.o pointT.o functions.o
	$(CC) -o $(BIN)/main $(OBJ)/main.o $(OBJ)/matrix.o $(OBJ)/point.o $(OBJ)/pointT.o $(OBJ)/functions.o $(RS2) $(OPT)

main.o: $(SRC)/main.cpp $(INCLUDE)/matrix.h $(INCLUDE)/point.h $(INCLUDE)/pointT.h $(INCLUDE)/functions.h
	$(CC) -c -o $(OBJ)/main.o $(SRC)/main.cpp -I$(INCLUDE) $(RS2) $(OPT)

matrix.o: $(SRC)/main.cpp $(INCLUDE)/matrix.h
	$(CC) -c -o $(OBJ)/matrix.o $(SRC)/matrix.cpp -I$(INCLUDE) $(OPT)

point.o: $(SRC)/point.cpp $(INCLUDE)/point.h
	$(CC) -c -o $(OBJ)/point.o $(SRC)/point.cpp -I$(INCLUDE) $(OPT)

pointT.o: $(SRC)/pointT.cpp $(INCLUDE)/pointT.h $(INCLUDE)/point.h
	$(CC) -c -o $(OBJ)/pointT.o $(SRC)/pointT.cpp -I$(INCLUDE) $(OPT)

functions.o: $(SRC)/functions.cpp $(INCLUDE)/functions.h $(INCLUDE)/point.h $(INCLUDE)/pointT.h
	$(CC) -c -o $(OBJ)/functions.o $(SRC)/functions.cpp -I$(INCLUDE) $(OPT)


clean:
	rm -rf $(OBJ)/*
	rm -rf $(BIN)/*

exec:
	./bin/emisorPC

install:
	# Utilizar la guia de instalacion de INTEL
