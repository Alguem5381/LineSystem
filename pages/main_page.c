/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <main_page.h>
#include <draw.h>
#include <letter.h>
#include <keys.h>

PageResult init_main_page(PageArgs args)
{
    PageResult result = {0};

    //Contextos
    DrawContext general_context =
    {
        .width = 100,
        .height = 100
    };
    DrawContext list_context = 
    {
        .element_in_focus = 0,
        .width = 90, 
        .height = 90
    };

    //Definição de estilo para os contextos
    set_style(args.style, &general_context);
    set_style(args.style, &list_context);

    //Vetores utilizados
    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Selecionar"};
    wchar_t *elements[] = {L"Procurar uma linha de ônibus", L"Manutenção do programa"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);
    int elements_length = sizeof(elements) / sizeof(elements[0]);

    // Para capitura teclas
    Key key = unknown;
    wint_t character;

    // Variaveis para o loop
    int need_draw = 1;
    int need_split = 1;
    int running = 1;
    int is_popup_on = args.throw_popup;


    while(running)
    {
        // Tratamento de teclas
        switch (key)
        {
        case resize:
            resize_term(0, 0);
            need_split = 1;
            need_draw = 1;
            break;

        case esc:
            result.action = page_action_back;
            running = 0;
            break;

        case up:
            if (list_context.element_in_focus > 0)
            {
                list_context.element_in_focus--;
                need_draw = 1;
            }
            break;

        case down:
            if (list_context.element_in_focus < elements_length - 1)
            {
                list_context.element_in_focus++;
                need_draw = 1;
            }
            break;

        case enter:
            result.action = page_action_select;
            result.selected_index = list_context.element_in_focus;
            running = 0;
            break;

        default:
            break;
        }

        // Definição das coordenadas dos contextos
        if (need_split)
        {
            general_context.startx = 0;
            general_context.starty = 0;
            general_context.endx = COLS;
            general_context.endy = LINES;

            split_context(&list_context, &general_context, 50, FIRST, HORIZONTAL);

            list_context.starty = general_context.starty + 4;
            list_context.endy = general_context.endy - 4;

            need_draw = 1;

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int sucessful = 1;
            sucessful *= draw_base_page(L"Menu principal", &general_context);
            sucessful *= draw_footer(keys, options, keys_length, &general_context);
            sucessful *= draw_list(elements, elements_length, &list_context);

            need_draw = 0;
            refresh();
        }

        // Capitura de tecla
        if (running)
        {
            int status = get_wch(&character);
            key = get_key(character, status);
        }
    }

    return result;
}