#ifndef SEARCH_PAGE_H
#define SEARCH_PAGE_H

#include <page_results.h>
#include <style.h>

PageResult init_search_page(Style const *style, void **persistence, wchar_t const *search_text, wchar_t const *time_text, wchar_t const *label_text, wchar_t *elements[], int const elements_length);

#endif //SEARCH_PAGE_H