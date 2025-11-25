#include <search.h>
#include <wchar.h>

int search_to_array(Object *information, wchar_t ***array, int *array_length, wchar_t *string){
    SLL_Node *node;

    for( node = information->SLL->head; node != NULL; node = node->next){
        if(wcscmp((wchar_t *)node->info, string)){
            wcscpy(**array, (wchar_t *) node->info);
            *array_length++;
        }
    }

    return (*array_length > 0);

}

wchar_t *search_route(Object *information, wchar_t *first_stop, wchar_t *second_stop, Hours departure_time, Hours arrival_time);
