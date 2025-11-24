#include <letter.h>

int is_letter(char const letter)
{
    if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z')
        return 1;

    return 0;
}

int is_wletter(wchar_t const letter)
{
    if (letter >= L'a' && letter <= L'z' || 
        letter >= L'A' && letter <= L'Z' ||
        letter >= L'À' && letter <= L'Ã' ||
        letter >= L'à' && letter <= L'ã' ||
        letter >= L'Ç' && letter <= L'Ê' ||
        letter >= L'ç' && letter <= L'ê' ||
        letter >= L'Ì' && letter <= L'Î' ||
        letter >= L'ì' && letter <= L'î' ||
        letter >= L'Ò' && letter <= L'Õ' ||
        letter >= L'ò' && letter <= L'õ' ||
        letter >= L'Ù' && letter <= L'Û' ||
        letter >= L'ù' && letter <= L'û'
    )
        return 1;
    return 0;
}

int is_number(char const letter)
{
    if (letter >= '0' && letter <= '9')
        return 1;
    return 0;
}

int is_wnumber(wchar_t const letter)
{
    if (letter >= L'0' && letter <= L'9')
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