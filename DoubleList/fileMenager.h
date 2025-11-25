#ifndef FILE_MENAGER_H
#define FILE_MENAGER_H

#define HANDLE_NAME "FileHandle.txt"

#include "simpleLinkedList.h"
#include "doubleList.h"
#include <stdio.h>

// Apenas aviso que Object existe — NÃO DEFINO.
typedef struct Object Object;

typedef struct SLL_Data {
    DoubleList *list;
    char name[10];
    char enterprise[15];
} BusLine;

typedef struct data_type {
    int id;
    char nome[10];
} BusStop;

typedef BusStop data_type;

// Funções do fileMenager
int loadFromFile(Object *object);
int saveToFile(Object *object);

int openFileHandle(FILE **file);
int handleManeger(Object *object, FILE *file);
int openLine(char *str, BusLine *line);
int saveStops(DoubleList *dl, char *path);
int removeLine(char *path);
int saveLine(Object *obj);

#endif
