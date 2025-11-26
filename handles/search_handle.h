#ifndef SEARCH_HANDLE_H
#define SEARCH_HANDLE_H

#include <states.h>
#include <style.h>

PageResult init_search_page(Style const *style, wchar_t const *search_text, wchar_t const *label_text, wchar_t const *elements, int const elements_length);

#endif //SEARCH_HANDLE_H