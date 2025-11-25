#include "doubleList.h"
#include <stdio.h>

typedef struct data_type {
  int value;
} BusLines;

/*int (BusLines *data1, BusLines *data2) {
  if (data1->value == data2->value)
    return 1;
  return 0;
}
*/

void showData(DataType *data) { printf("%d - ", data->value); }

int test() {

  DoubleList L;

  create(&L);

  BusLines b1;
  b1.value = 1;
  BusLines b2;
  b2.value = 2;
  BusLines b3;
  b3.value = 3;
  BusLines b4;
  b4.value = 4;

  // Adicionando
  add(&L, &b1);
  add(&L, &b2);
  add(&L, &b3);
  add(&L, &b4);

  show(&L);
  printf("\n\n");

  // Removendo
  removeData(&L, &b2);
  printf("removeu 2\n");
  show(&L);
  printf("\n\n");

  removeData(&L, &b3);
  printf("removeu 3\n");
  show(&L);
  printf("\n\n");

  // Procurando
  printf("Procurando por %d\n", b4.value);
  const DataType *d = search(&L, &b4);

  if(d == NULL)
      printf("Nao achou\n");
  else
      printf("Achou %d\n", d->value);

  BusLines b5;
  b5.value = 5;

  printf("Procurando por %d\n", b5.value);
  const DataType *d2 = search(&L, &b5);

  if(d2 == NULL)
      printf("Nao achou\n");
  else
      printf("Achou %d\n", d2->value);

  return 0;
}

