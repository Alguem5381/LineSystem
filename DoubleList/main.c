#include "doubleList.h"
#include <stdio.h>

typedef struct data_type {
  int value;
} BusLines;

int comp(BusLines *data1, BusLines *data2) {
  if (data1->value == data2->value)
    return 1;
  return 0;
}

void showData(DataType *data) { printf("%d - ", data->value); }