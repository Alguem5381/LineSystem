#include "doubleList.h"
#include <stdio.h>
#include <stdlib.h>

void create(DoubleLinkedList *list) {
  list->head = NULL;
  list->size = 0;
}

int append(DoubleLinkedList *dl, DataType *data)
{
    if (!dl || !data) return 0;

    DoubleLinkedListNode *new_node = malloc(sizeof(DoubleLinkedListNode));
    if (!new_node) return 0;

    new_node->info = data;

    if (dl->head == NULL) 
    {
        dl->head = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
    } else 
    {
        DoubleLinkedListNode *tail = dl->head->prev;

        new_node->next = dl->head;
        new_node->prev = tail;

        tail->next = new_node;
        dl->head->prev = new_node;
    }

    dl->size++;
    return 1;
}

void removeNode(DoubleLinkedListNode *n) {
  n->prev->next = n->next;
  n->next->prev = n->prev;
  free(n);
}

int isEmpty(DoubleLinkedList *list) { return (list->size == 0); }
