#ifndef NEWSTOP_HANDLE_H
#define NEWSTOP_HANDLE_H

#include <states.h>
#include <style.h>
#include <object.h>

HandleResult init_newstop_handle(Style const *style, SimpleLinkedListNode *line_node, DoubleLinkedListNode *prev_node);

#endif //NEWSTOP_HANDLE_H