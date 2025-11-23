#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <template_page.h>
#include <draw.h>

void set_style(Style const *style, DrawContext *general_context)
{
    general_context->default_border_color = style->default_border_color;
    general_context->default_pair_color = style->default_pair_color;
    general_context->divider_color = style->divider_color;
    general_context->on_focus_border_color = style->on_focus_border_color;
    general_context->on_focus_pair_color = style->on_focus_pair_color;
}

//Inicializador da página

PageResult init_template_page(Style const *style)
{
    PageResult result = {0};

    //Contextos
    DrawContext general_context =
    {
        .startx = 0,
        .starty = 0,
        .width = 100,
        .height = 100
    };

    //Definição de estilo para os contextos
    set_style(style, &general_context);

    //Vetores utilizados
    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Selecionar"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    //Variaveis para o loop
    int need_draw = 1;
    int running = 1;
    int selected = 0;
    int character = KEY_RESIZE;

    while(running)
    {
        switch (character)
        {
        case KEY_RESIZE:
        {
            resize_term(0, 0);
            general_context.endx = COLS;
            general_context.endy = LINES;

            need_draw = 1;

            break;
        }

        case 27:
            result.action = page_action_back;
            running = 0;
            break;

        default:
            break;
        }

        if (need_draw)
        {
            int sucessful = 1;
            sucessful *= draw_base_page(L"Título", &general_context);
            sucessful *= draw_footer(keys, options, keys_length, &general_context);
            need_draw = 0;
            refresh();
        }

        if(running)
            character = getch();
    }

    return result;
}