#include "simpleLinkedList.h"

int get_size_sll(SimpleLinkedList *L){
    int count;
    SimpleLinkedListNode *h;
    for(h=L->head, count = 0; h!= NULL; h=h->next, count++)
    return count;
}

int define_sll(SimpleLinkedList* L) {
    L->head = NULL;
    return 1;
}

int empty(SimpleLinkedList *L){
    return(!L->head);
}

int init_insert_sll(SimpleLinkedList *L, void *V){

    SimpleLinkedListNode *p;
    p = (SimpleLinkedListNode*)malloc(sizeof(SimpleLinkedListNode));

    if(!p)
        return 0;

    p->info = V;
    p->next = L->head;
    L->head = p;
    return 1;
}

int end_insert_sll(SimpleLinkedList *L, void *V){
    if(empty(L))
        return (init_insert_sll(L, V));
    SimpleLinkedListNode *p = L->head;
    SimpleLinkedListNode *q;
    while (p->next!= NULL)
        p = p->next;
    q = (SimpleLinkedListNode*) malloc(sizeof(SimpleLinkedListNode));
    if(!q) return 0;
    q->info;
    q->next = NULL;
    p->next = q;
    
    return 1;
}

int init_remove_sll(SimpleLinkedList *L){
    if(empty(L))
        return 0;
    SimpleLinkedListNode *p = L->head;
    L->head = p->next;
    free(p);
    return 1;
}

int endRemove_rec(SimpleLinkedList *L, SimpleLinkedListNode **q, SimpleLinkedListNode **p){
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

int end_remove_sll(SimpleLinkedList *L){
    SimpleLinkedListNode *p, *q;
    p = L->head;
    q = NULL;
    while (p->next!=NULL)   
    return(endRemove_rec(L, &q, &p));
}

void destroy_rec(SimpleLinkedListNode **p){
    if (*p == NULL) return;
    destroy_rec(&((*p)->next));
    free(*p);
    *p = NULL;
}

void destroy_sll(SimpleLinkedList *L){
    destroy_rec(&(L->head));
}
