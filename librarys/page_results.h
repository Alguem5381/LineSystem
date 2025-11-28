/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef PAGERESULTS_H
#define PAGERESULTS_H

#include <style.h>
#include <wchar.h>

#define DBL 256

/// @brief Enumerador para ações que uma página pode retornar
typedef enum PageAction
{
    page_action_fail,
    page_action_back,
    page_action_select,
    page_action_text,
    page_action_text_and_selected
} PageAction;

/// @brief Estrutura para retornar os resultados de uma página
typedef struct PageResult
{
    PageAction action;
    int selected_index;
    wchar_t first_text[DBL];
    wchar_t second_text[DBL];
    wchar_t third_text[DBL];
} PageResult;

/// @brief Argumentos necessários para uma página
typedef struct PageArgs 
{
    const Style *style;
    void **persistence;
    int state;
    int throw_popup;
} PageArgs;


#endif //PAGERESULTS_H