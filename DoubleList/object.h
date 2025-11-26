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

int deleteObject(Object *object);
int deleteLine(DoubleList *dl);
int removeByName(Object *obj, char *name);
int insertBusStop(Object *obj, char *lineName, int id, char *name);
int insertBusLine(Object *obj, char *name, char *enterprise);
int hasBusLine(Object *obj, char *name);
int removeStop(Object *obj, char *lineName, char *stopName);

#endif
