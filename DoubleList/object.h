#ifndef OBJECT_H
#define OBJECT_H

#include "simpleLinkedList.h"
#include "doubleList.h"


typedef struct Object {
    linkedList *SLL;
} Object;


int defineObject(Object *object);
int loadData(Object *object);
int saveObject(Object *object);

//WIP
int deleteObject(Object *object);
int deleteLine(DoubleList *dl);

#endif
