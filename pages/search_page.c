#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <search_page.h>
#include <draw.h>

//Inicializador da página

PageResult init_search_page(Style const *style, wchar_t const *search_text, wchar_t const *time_text, wchar_t const *label_text, wchar_t *elements[], int const elements_length)
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

    DrawContext list_context = {
        .height = 100,
        .width = 90
    };

    DrawContext search_bar_context = {
        .height = 100,
        .width = 90
    };

    DrawContext time_bar_context = {
        .height = 100,
        .width = 90
    };

    DrawContext label_context = {
        .height = 100,
        .width = 90
    };

    //Definição de estilo para os contextos
    set_style(style, &general_context);
    set_style(style, &list_context);
    set_style(style, &search_bar_context);
    set_style(style, &time_bar_context);
    set_style(style, &label_context);

    //Vetores utilizados
    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Selecionar"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    //Variaveis para o loop
    int need_draw = 1;
    int running = 1;
    int selected = 0;

    //Para capituras de teclas
    int status = 256;
    wint_t character = KEY_RESIZE;

    while(running)
    {
        if (status) //Teclas especias vão aqui
        {
            switch (character)
            {
            case KEY_RESIZE:
            {
                resize_term(0, 0);
                general_context.endx = COLS;
                general_context.endy = LINES;

                DrawContext main_panel_context = general_context;
                DrawContext left_panel_context;
                DrawContext right_panel_context;

                main_panel_context.starty += 5;
                main_panel_context.endy -= 5;

                split_context(&left_panel_context, &main_panel_context, 50, 0, 0);
                split_context(&right_panel_context, &main_panel_context, 50, 1, 0);

                split_context(&list_context, &left_panel_context, 20, 1, 1);
                split_context(&search_bar_context, &left_panel_context, 30, 0, 1);
                split_context(&time_bar_context, &right_panel_context, 30, 0, 1);
                split_context(&label_context, &right_panel_context, 20, 1, 1);

                need_draw = 1;

                break;
            }

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
        else        //Teclas comuns
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
                sucessful *= draw_base_page(L"Buscar rotas - origem", &general_context);
                sucessful *= draw_footer(keys, options, keys_length, &general_context);
                sucessful *= draw_text_box(search_text, &search_bar_context);
                sucessful *= draw_text_box(time_text, &time_bar_context);
                sucessful *= draw_label(label_text, &label_context);
                sucessful *= draw_list(elements, elements_length, &list_context);

                need_draw = 0;
                refresh();
            }

        if(running)
            status = get_wch(&character);
    }

    return result;
}