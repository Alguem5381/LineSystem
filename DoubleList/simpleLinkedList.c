#include "simpleLinkedList.h"

int size(linkedList *L){
    int count;
    SLL_Node *h;
    for(h=L->head, count = 0; h!= NULL; h=h->next, count++)
    return count;
}

int define(linkedList* L) {
    L->head = NULL;
    return 1;
}

int empty(linkedList *L){
    return(!L->head);
}

int initInsert(linkedList *L, void *V){

    SLL_Node *p;
    p = (SLL_Node*)malloc(sizeof(SLL_Node));
    if(!p)
        return 0;
    p->info = V;
    p->next = L->head;
    L->head = p;
    return 1;

}

int endInsert(linkedList *L, void *V){
    if(empty(L))
        return (initInsert(L, V));
    SLL_Node *p = L->head;
    SLL_Node *q;
    while (p->next!= NULL)
        p = p->next;
    q = (SLL_Node*) malloc(sizeof(SLL_Node));
    if(!q) return 0;
    q->info;
    q->next = NULL;
    p->next = q;
    
    return 1;
}

int initRemove(linkedList *L){
    if(empty(L))
        return 0;
    SLL_Node *p = L->head;
    L->head = p->next;
    free(p);
    return 1;
}

int endRemove_rec(linkedList *L, SLL_Node **q, SLL_Node **p){
    if(p==NULL)
        return 0;
    if((*p)->next == NULL){
        if(q != NULL)
            (*q)->next = NULL;
        else L->head = NULL;
    free(*p);
    return 1;
    }
    return(endRemove_rec(L,&(*p), &(*p)->next));

}

int endRemove(linkedList *L){
    SLL_Node *p, *q;
    p = L->head;
    q = NULL;
    while (p->next!=NULL)   
    return(endRemove_rec(L, &q, &p));
}



void destroy_rec(SLL_Node **p){
    if (*p == NULL) return;
    destroy_rec(&((*p)->next));
    free(*p);
    *p = NULL;
}

void destroy(linkedList *L){
    destroy_rec(&(L->head));
}
