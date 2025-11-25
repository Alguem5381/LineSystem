#include <hours.h>
#include <letter.h>

int is_valid_time(const wchar_t *string)
{
    int length = wcslen(string);

    //"19h30m" possui tamanho 6
    if (length != 6) return 0;

    if (is_number(string[0]) &&
        is_number(string[1]) &&
        (string[2] == L'h' || string[2] == L'H') &&
        is_number(string[3]) &&
        is_number(string[4]) &&
        (string[5] == L'm' || string[5] == L'M'))
        return 1;

    return 0;
}

int time_to_string(wchar_t *string, const int string_length, const Hours time)
{

}

int string_to_time(const wchar_t *string, Hours *time)
{

}

int get_next_nearest_time(Hours *array, int array_lenght, Hours target)
{

}