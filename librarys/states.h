#ifndef STATES_H
#define STATES_H

#include <wchar.h>

typedef enum States
{
    state_exit,
    state_main,
    state_search,
    state_login,
    state_line,
    state_new_line,
    state_stops,
    state_new_stop,
    state_edit_stop
} States;

typedef struct HandleResult
{
    wchar_t *first_value;
    wchar_t *second_value;
    States state;
} HandleResult;

#endif //STATES_H