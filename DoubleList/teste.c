
#include "object.h"
#include "fileMenager.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// int hasBusLine(Object *obj, char *name){
//     SLL_Node *curr = obj->SLL->head;
//     while (curr)
//     {
//         if (!strcmp((((BusLine*)(curr->info))->name),name ))
//             return 1;
//         curr = curr->next;
//     }
//     return 0;
// }

int comp(DataType *data1, DataType *data2)
{
    if (data1->id == data2->id)
        return 1;
    return 0;
}
void showData(DataType *data) { printf("%d - ", data->id); }

int main()
{
    
    
    Object obj;
    defineObject(&obj);

    loadData(&obj);

    int n;
    scanf("%d", &n);
    //insertBusLine(&obj, "12345", "Latam");
    // printf("%d", insertBusStop(&obj, "12345", 94, "São Paulo"));
    //insertBusStop(&obj, "12345", 52, "Barra");
    // insertBusStop(&obj, "12345", 54, "Limeira");

    // removeStop(&obj, "12345", "Barra");

    //saveObject(&obj);
    // deleteObject(&obj);

    // Object qq;
    // defineObject(&qq);
    // loadData(&qq);

    
    SLL_Node *temp = obj.SLL->head;
    while (temp)
    {
        BusLine *bus_line = (BusLine*)temp->info;
        printf("%s - %s\n", bus_line->name, bus_line->enterprise);
        //show(bus_line->list);
        temp = temp->next;
    }

    //removeByName(&obj, "45678");

    //saveObject(&obj);
    
    // scanf("%d", &n);

    

    // deleteObject(&obj);

    // SLL_Node *jdj = obj.SLL->head;
    // while (jdj)
    // {
    //     BusLine *bus_line = (BusLine*)jdj->info;
    //     printf("%s - %s\n", bus_line->name, bus_line->enterprise);
    //     show(bus_line->list);
    //     jdj = jdj->next;
    // }

    return 0;
}
