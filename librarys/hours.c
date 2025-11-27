#include <hours.h>
#include <letter.h>

int is_valid_time(const Hours time)
{
    if (time.hours < 0 || time.hours > 23 || time.minutes < 0 || time.minutes > 59) return 0;

    return 1;
}

int is_valid_format(const wchar_t *string)
{
    //"19h30m" possui tamanho 6, '\0' não conta
    if (!string || wcslen(string) != 6) return 0;

    if (is_number(string[0]) &&
        is_number(string[1]) &&
        (string[2] == L'h' || string[2] == L'H') &&
        is_number(string[3]) &&
        is_number(string[4]) &&
        (string[5] == L'm' || string[5] == L'M'))
        return 1;

    return 0;
}

int time_to_string(wchar_t *buffer, const int buffer_length, const Hours time)
{
    if (buffer_length < 7 || !buffer || !is_valid_time(time)) return 0;

    swprintf(buffer, buffer_length, L"%02dh%02dm", time.hours, time.minutes);

    return 1;
}

int string_to_time(const wchar_t *string, Hours *time)
{
    if (!time || !string || !is_valid_format(string)) return 0;

    Hours temp;

    temp.hours = (string[0] - L'0') * 10 + string[1] - L'0';
    temp.minutes = (string[3] - L'0') * 10 + string[4] - L'0';

    if (!is_valid_time(temp)) return 0;

    *time = temp;

    return 1;
}

int to_minutes(Hours h) 
{
    return h.hours * 60 + h.minutes;
}