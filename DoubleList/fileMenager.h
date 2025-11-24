#ifndef FILE_MENAGER_H
#define FILE_MENAGER_H
#define HANDLE_NAME "FileHandle.txt"
#include "simpleLinkedList.h"
#include "doubleList.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct SLL_Data{
    DoubleList *list;
    char name[10];
    char enterprise[15];
}BusLine;

typedef struct data_type
{
    int id;
    char nome[10];
}BusStop;

typedef BusStop data_type;

typedef struct Object
{
    linkedList *SLL;
}Object;


int createObject(Object *obj);
int init(Object *object);
int openFileHandle(FILE **file);
int handleManeger(Object *object, FILE *file);
int addList(Object *object, DoubleList value);
int openLine(char *str, BusLine *list);
int saveObject(Object *obj);
int saveLine(linkedList *ll);
int saveStops(DoubleList *dl, char *path);
int removeLine(char *path);

#endif