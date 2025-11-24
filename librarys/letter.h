#ifndef LETTER_H
#define LETTER_H

#include <wchar.h>

int is_letter(char const letter);

int is_wletter(wchar_t const letter);

int is_number(char const letter);

int is_wnumber(wchar_t const letter);

int remove_lastw(wchar_t string[]);

#endif //LETTER_H