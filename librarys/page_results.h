#ifndef PAGERESULTS_H
#define PAGERESULTS_H

typedef enum PageAction
{
    page_action_back,
    page_action_select,
    page_action_text
} PageAction;

typedef struct PageResult
{
    PageAction action;
    int selected_index;
    char text[256];
} PageResult;


#endif //PAGERESULTS_H