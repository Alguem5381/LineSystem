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
        DoubleList *DL = (DoubleList*)(curr->info);
        deleteLine(DL);

        prev = curr;
        curr = curr->next;
        free(prev);
    }

    obj->SLL = NULL;
    return 1;
}

int deleteLine(DoubleList *dl){
    if (!dl || !dl->head) return 0;
    
    Node *curr = dl->head;
    Node *prev;

    while (curr)
    {
        prev = curr;
        curr = curr->next;
        free(prev);
    }

    dl->head = NULL;
    return 1;
}
