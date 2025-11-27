#ifndef EDITSTOP_HANDLE_H
#define EDITSTOP_HANDLE_H

#include <states.h>
#include <style.h>
#include <object.h>

HandleResult init_editstop_handle(Style const *style, DoubleLinkedListNode *target_node, SimpleLinkedListNode *line_node);

#endif //EDITSTOP_HANDLE_H