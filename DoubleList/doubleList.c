#include "doubleList.h"
#include <stdio.h>
#include <stdlib.h>

void create(DoubleLinkedList *list) {
  list->head = NULL;
  list->size = 0;
}

int add(DoubleLinkedList *list, DataType *data) {
  DoubleLinkedListNode *n = (DoubleLinkedListNode *)malloc(sizeof(DoubleLinkedListNode));

  if (n == NULL)
    return 0;

  n->info = data;

  if (isEmpty(list)) {
    list->head = n;
    n->next = list->head;
    n->prev = list->head;
  } else {
    n->next = list->head;

    list->head->prev->next = n;
    n->prev = list->head->prev;

    list->head->prev = n;
    n->next = list->head;

    list->head = n;
  }
  list->size += 1;
  return 1;
};

void removeNode(DoubleLinkedListNode *n) {
  n->prev->next = n->next;
  n->next->prev = n->prev;
  free(n);
}

int isEmpty(DoubleLinkedList *list) { return (list->size == 0); }
