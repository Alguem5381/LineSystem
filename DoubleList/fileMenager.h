#ifndef FILE_MENAGER_H
#define FILE_MENAGER_H

#define HANDLE_NAME "FileHandle.txt"

#include "simpleLinkedList.h"
#include "doubleList.h"
#include <stdio.h>
#include <string.h>


typedef struct Object Object;

typedef struct SLL_Data {
    DoubleList *list;
    char name[20];
    char enterprise[20];
} BusLine;

typedef struct data_type {
    int id;
    char nome[20];
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
