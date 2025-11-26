#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <states.h>
#include <style.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

//Handles
#include <main_handle.h>
#include <search_handle.h>
#include <login_handle.h>
#include <line_handle.h>
#include <stop_handle.h>
#include <newline_handle.h>
#include <newstop_handle.h>
#include <editstop_handle.h>

int init_ncurses()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    initscr();
    cbreak();
    curs_set(0);
    noecho();
    keypad(stdscr, true);
    start_color();

    if(!has_colors())
    {
        printf("Não suporta cores\n");
        return 0;
    }

    return 1;
}

int end_ncurses()
{
    curs_set(1);
    endwin();

    return 1;
}

int init_style(Style *style)
{
    if (!style) return 0;

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_CYAN, COLOR_CYAN);
    init_pair(4, COLOR_WHITE, COLOR_CYAN);
    init_pair(5, COLOR_WHITE, COLOR_CYAN);

    style->default_pair_color = 1;
    style->default_border_color = 2;
    style->on_focus_border_color = 3;
    style->on_focus_pair_color = 4;
    style->divider_color = 5;

    return 1;
}

int main()
{
    //Structs
    Style style;

    //Funções de configuração do programa e structs 

    int result = 1;

    if (!init_ncurses())
    {
        printf("Falha ao iniciar o ncurses\n");
        result = 0;
    }

    if (!init_style(&style))
    {
        printf("Falha ao iniciar o estilo\n");
        result = 0;
    }

    //Loop principal

    int running = result;

    HandleResult handle_result = {
        .state = state_main,
        .first_value = NULL
    };

    while(running)
    {
        switch (handle_result.state)
        {
        case state_main:
            handle_result = init_main_handle(&style);
            break;

        case state_search:
            handle_result = init_search_handle(&style);
            break;

        case state_login:
            handle_result = init_login_handle(&style);
            break;

        case state_line:
            handle_result = init_line_handle(&style);
            break;

        case state_stops:
            handle_result = init_stop_handle(&style, handle_result.first_value);
            break;

        case state_new_line:
            handle_result = init_newline_handle(&style);
            break;

        case state_new_stop:
            handle_result = init_newstop_handle(&style, handle_result.first_value);
            break;

        case state_edit_stop:
            handle_result = init_editstop_handle(&style, handle_result.first_value, handle_result.second_value);
            break;

        case state_exit:
            running = 0;
            break;
        
        default:
            break;
        }
    }

    //Funções de finalização

    free(handle_result.first_value);
    free(handle_result.second_value);

    end_ncurses();

    return 0;
}