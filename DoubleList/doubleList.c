#include "doubleList.h"
#include <stdio.h>
#include <stdlib.h>

void create(DoubleList *list) {
  list->head = NULL;
  list->size = 0;
}

int add(DoubleList *list, DataType *data) {
  Node *n = (Node *)malloc(sizeof(Node));

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

int removeData(DoubleList *list, DataType *data) {
  if (isEmpty(list))
    return -1;

  Node *n = list->head;

  for (int i = 0; i < list->size; i++, n = n->next) {
    if (comp(n->info, data)) {
      if (list->size == 2)
        list->head = n->next;
      else if (list->size == 1)
        list->head = NULL;
      removeNode(n);
      list->size -= 1;
      return 1;
    }
  }

  return 0;
}

void removeNode(Node *n) {
  n->prev->next = n->next;
  n->next->prev = n->prev;
  free(n);
}

DataType const *search(DoubleList *list, DataType *data){
  Node *n = list->head;

  for (int i = 0; i < list->size; i++, n = n->next) {
      if(comp(n->info, data))
          return n->info;
  }

  return NULL;
}

int isEmpty(DoubleList *list) { return (list->size == 0); }

void show(DoubleList *list) {
  if (isEmpty(list))
    return;

  Node *n = list->head;

  for (int i = 0; i < list->size; i++, n = n->next)
    showData(n->info);
  printf("\n");
}

