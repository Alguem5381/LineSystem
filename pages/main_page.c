#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <main_page.h>
#include <draw.h>

//Inicializador da página

PageResult init_main_page(Style const *style, void **persistence)
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
    DrawContext list_context = 
    {
        .element_in_focus = 0,
        .width = 90, 
        .height = 90
    };

    //Definição de estilo para os contextos
    set_style(style, &general_context);
    set_style(style, &list_context);

    //Vetores utilizados
    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Selecionar"};
    wchar_t *elements[] = {L"Procurar uma linha de ônibus", L"Manutenção do programa"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);
    int elements_length = sizeof(elements) / sizeof(elements[0]);

    //Variaveis para o loop
    int need_draw = 1;
    int running = 1;
    int selected = 0;

    int status = 256;
    wint_t character = KEY_RESIZE;

    while(running)
    {
        if (status == KEY_CODE_YES) //Teclas especiais vão aqui
        {
            switch (character)
            {
            case KEY_RESIZE:
            {
                resize_term(0, 0);
                general_context.endx = COLS;
                general_context.endy = LINES;

                split_context(&list_context, &general_context, 50, 0, 0);

                list_context.starty = general_context.starty + 4;
                list_context.endy = general_context.endy - 4;

                need_draw = 1;

                break;
            }

            case '\n':
                result.action = page_action_select;
                result.selected_index = list_context.element_in_focus;
                running = 0;
                break;

            case KEY_UP:
                if (list_context.element_in_focus > 0)
                {
                    list_context.element_in_focus--;
                    need_draw = 1;
                }
                break;

            case KEY_DOWN:
                if (list_context.element_in_focus < elements_length - 1)
                {
                    list_context.element_in_focus++;
                    need_draw = 1;
                }
                break;

            default:
                break;
            }
        }
        else    //Aqui vão as teclas comuns
        {
            switch (character)
            {
            case 27:
                result.action = page_action_back;
                running = 0;
                break;
            
            default:
                break;
            }
        }

        if (need_draw)
        {
            int sucessful = 1;
            sucessful *= draw_base_page(L"Menu principal", &general_context);
            sucessful *= draw_footer(keys, options, keys_length, &general_context);
            sucessful *= draw_list(elements, elements_length, &list_context);
            need_draw = 0;
            refresh();
        }

        if(running)
            status = get_wch(&character);
    }

    return result;
}