/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include <letter.h>
#include <stdlib.h>

int is_letter(char const character)
{
    if (character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z' || character == ' ')
        return 1;

    return 0;
}

int is_wletter(wchar_t const character)
{
    if (character >= L'a' && character <= L'z' || 
        character >= L'A' && character <= L'Z' ||
        character >= L'À' && character <= L'Ã' ||
        character >= L'à' && character <= L'ã' ||
        character >= L'Ç' && character <= L'Ê' ||
        character >= L'ç' && character <= L'ê' ||
        character >= L'Ì' && character <= L'Î' ||
        character >= L'ì' && character <= L'î' ||
        character >= L'Ò' && character <= L'Õ' ||
        character >= L'ò' && character <= L'õ' ||
        character >= L'Ù' && character <= L'Û' ||
        character >= L'ù' && character <= L'û' ||
        character == L' '
    )
        return 1;
    return 0;
}

int is_number(char const character)
{
    if (character >= '0' && character <= '9')
        return 1;
    return 0;
}

int is_wnumber(wchar_t const character)
{
    if (character >= L'0' && character <= L'9')
        return 1;
    return 0;
}

int remove_lastw(wchar_t string[])
{
    int length = wcslen(string);

    if (!length)
        return 0;

    string[length - 1] = L'\0';
    return 1;
}

int add_lastw(wchar_t string[], int buffer_length, wchar_t character)
{
    if (wcslen(string) >= buffer_length)
        return 0;

    wcsncat(string, &character, 1);
    return 1;
}

int is_emptyw(wchar_t *string)
{
    return string[0] == L'\0';
}

int contains_wstr(wchar_t *needle, wchar_t *haystack)
{
    if (!haystack || !needle)
        return 0;

    if (wcsstr(haystack, needle) != NULL)
        return 1;
    return 0;
}

void free_string_array(wchar_t **string_array, int length) {
    if (!string_array) return;

    for (int i = 0; i < length; i++) {
        if (string_array[i]) {
            free(string_array[i]);
        }
    }
    free(string_array);
}