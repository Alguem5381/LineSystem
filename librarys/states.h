#ifndef STATES_H
#define STATES_H

typedef enum States
{
    state_exit,
    state_main,
    state_search,
    state_login
} States;

typedef struct HandleResult
{
    int value;
    States state;
} HandleResult;

#endif //STATES_H