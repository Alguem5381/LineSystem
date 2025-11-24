#ifndef PAGERESULTS_H
#define PAGERESULTS_H

#include <wchar.h>

typedef enum PageAction
{
    page_action_fail,
    page_action_back,
    page_action_select,
    page_action_text,
    page_action_text_and_selected
} PageAction;

typedef struct PageResult
{
    PageAction action;
    int selected_index;
    wchar_t text[256];
} PageResult;


#endif //PAGERESULTS_H