#include "object.h"
#include "fileMenager.h"

int defineObject(Object *object)
{
    if (!object) return 0;

    linkedList *LL = malloc(sizeof(linkedList));
    if (!LL) return 0;

    define(LL);
    object->SLL = LL;

    return 1;
}

int loadData(Object *object) {
    if (!object) return 0;
    return loadFromFile(object);  
}

int saveObject(Object *object) {
    if (!object || !object->SLL) return 0;
    return saveToFile(object); 
}

int deleteObject(Object *obj){
    if (!obj || !obj->SLL) return 0;
    
    SLL_Node *curr = obj->SLL->head;
    SLL_Node *prev;

    while (curr)
    {
        BusLine *DL = ((BusLine*)curr->info);
        printf("%s\n", DL->name);
        curr = curr->next;
    }
    destroy(obj->SLL);

    return 1;
}

int deleteLine(DoubleList *dl) {
    if (!dl || !dl->head) return 0;

    Node *curr = dl->head;
    Node *next;

    do {
        next = curr->next;
        
        free(curr);
        curr = next;
    } while (curr != dl->head);

    dl->head = NULL;
    dl->size = 0;

    return 1;
}

int removeByName(Object *obj, char *name) {
    if (!obj || !obj->SLL || !obj->SLL->head) return 0;

    SLL_Node *curr = obj->SLL->head;
    SLL_Node *prev = NULL;

    while (curr) {
        BusLine *line = (BusLine*)curr->info;

        if (line && !strcmp(line->name, name)) {
            deleteLine(line->list);

            if (prev == NULL) {
                obj->SLL->head = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(line);
            free(curr);

            return 1;
        }

        prev = curr;
        curr = curr->next;
    }

    return 0;
}



int insertBusLine(Object *obj, char *name, char *enterprise)
{
    BusLine *bus = malloc(sizeof(BusLine));
    if(!bus)
        return 0;
    DoubleList *DL = malloc(sizeof(DoubleList));
    if(!DL)
        return 0;
    create(DL);
    if(hasBusLine(obj,name))
        return 0;
    strcpy(bus->enterprise, enterprise);
    strcpy(bus->name, name);
    bus->list = DL;
    if(!initInsert(obj->SLL, bus)){
        free(bus);
        return 0;
    }
    return 1;
}

int insertBusStop(Object *obj, char *lineName, int id, char *name)
{
    if (!obj || !obj->SLL) return 0;

    BusStop *stop = malloc(sizeof(BusStop));
    if (!stop) return 0;

    stop->id = id;
    strcpy(stop->nome, name);

    SLL_Node *curr = obj->SLL->head;

    while (curr) {
        BusLine *line = (BusLine*) curr->info;

        if (line && !strcmp(line->name, lineName)) {

            if (!add(line->list, stop)) {
                free(stop);
                return 0;
            }

            return 1;
        }

        curr = curr->next;
    }

    free(stop);
    return 0;
}


int hasBusLine(Object *obj, char *name){
    SLL_Node *curr = obj->SLL->head;
    while (curr)
    {
        if (!strcmp((((BusLine*)(curr->info))->name),name ))
            return 1;
        curr = curr->next;
    }
    return 0;
}