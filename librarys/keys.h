#ifndef KEYS_H
#define KEYS_H

#include <wchar.h>

typedef enum Key
{
    unknown,
    esc,
    resize,
    backspace,
    enter,
    up,
    down,
    left,
    right,
    number,
    common
} Key;

Key get_key(wint_t key, int status);

#endif //KEYS_H