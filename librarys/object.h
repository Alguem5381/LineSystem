/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef OBJECT_H
#define OBJECT_H

#include "simpleLinkedList.h"
#include "doubleList.h"
#include <hours.h>

typedef struct SimpleLinkedListData {
    DoubleLinkedList *list;
    wchar_t name[20];
    wchar_t enterprise[20];
} BusLine;

typedef struct DoubleLinkedListData {
    Hours departure_time;
    Hours arrival_time;
    wchar_t nome[20];
} BusStop;

typedef struct Object {
    SimpleLinkedList *SLL;
} Object;

// Struct para devolver o resultado de buscas
typedef struct RouteResult {
    wchar_t line_name[20];
    wchar_t enterprise[20];
    wchar_t origin_name[20];
    wchar_t dest_name[20];
    Hours departure; // Saída da Origem
    Hours arrival;   // Chegada no Destino
    int found;       // 1 se achou, 0 se não
} RouteResult;

int defineObject(Object *object);
int loadData(Object *object);
int saveObject(Object *object);

int deleteObject(Object *object);
int insertBusLine(Object *obj, wchar_t *name, wchar_t *enterprise);
int hasBusLine(Object *obj, wchar_t *name);
int insertStopAfter(BusLine *line, DoubleLinkedListNode *prev_node, BusStop *new_data);
int removeStopNode(BusLine *line, DoubleLinkedListNode *target_node);
int removeLineNode(Object *obj, SimpleLinkedListNode *target_node);

int get_stops_to_array(SimpleLinkedListNode *list_to_search, DoubleLinkedListNode ***array_out, int *array_length, wchar_t *search_term);
int get_all_stops_to_array(Object *obj, DoubleLinkedListNode ***array_out, int *array_length, wchar_t *search_term);
int get_lines_to_array(Object *obj, SimpleLinkedListNode ***array_out, int *array_length, wchar_t *search_term);
wchar_t **create_stop_strings(DoubleLinkedListNode **node_array, int length);
wchar_t **create_line_strings(SimpleLinkedListNode **node_array, int length);
RouteResult find_best_route(Object *obj, wchar_t *origin_name, wchar_t *dest_name, Hours target_arrival);
wchar_t **create_unique_names_list(Object *data, int *out_length, wchar_t *search_term);

#endif
