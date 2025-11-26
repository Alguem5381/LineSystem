#ifndef LETTER_H
#define LETTER_H

#include <wchar.h>

int is_letter(char const character);

int is_wletter(wchar_t const character);

int is_number(char const character);

int is_wnumber(wchar_t const character);

int remove_lastw(wchar_t string[]);

int add_lastw(wchar_t string[], int length, wchar_t character);

int is_emptyw(wchar_t string[]);

int contains_wstr(wchar_t *needle, wchar_t *haystack);

#endif //LETTER_H