#ifndef PAGERESULTS_H
#define PAGERESULTS_H

#include <style.h>
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

typedef struct PageArgs 
{
    Style *style;
    void **persistence;
    int state;
    int throw_popup;
} PageArgs;


#endif //PAGERESULTS_H