#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <keys.h>
#include <letter.h>

Key get_key(wint_t key, int status)
{
    // Tratamento de tecla
    if (status) // Teclas especias vão aqui
    {
        switch (key)
        {
        case KEY_RESIZE:
            return resize;

        case KEY_UP:
            return up;

        case KEY_DOWN:
            return down;

        case KEY_LEFT:
            return left;

        case KEY_RIGHT:
            return right;

        case KEY_BACKSPACE:
            return backspace;

        default:
            return unknown;
        }
    }
    else // Teclas comuns
    {
        switch (key)
        {
        case 27:
            return esc;

        case '\n':
            return enter;

        case 127:
        case '\b':
        return backspace;

        default:
            if (is_wletter((wchar_t)key))
                return common;

            return unknown;
        }
    }
}