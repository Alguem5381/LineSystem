#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <search_page.h>
#include <draw.h>
#include <stdlib.h>

//Struct para memória persistente

typedef struct Persistence
{
    int list_select;
    int is_search_bar_selected;
} Persistence;


PageResult init_search_page(Style const *style, void **persistence, int state, wchar_t const *label_text, wchar_t *elements[], int const elements_length)
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
        .width = 90,
        .element_in_focus = 1
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

    //Usando a memória persistente
    Persistence *memory = NULL;

    if ((*persistence))
    {
        memory = (Persistence*)*persistence;

        list_context.element_in_focus = memory->list_select;
        if (!memory->is_search_bar_selected)
        {
            search_bar_context.element_in_focus = 0;
            time_bar_context.element_in_focus = 1;
        }
    }

    while(running)
    {
        //Tratamento de tecla
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

            case '\n': 
                result.action = page_action_select;
                running = 0;
                break;
            
            default:
                break;
            }
        }

        //Desenho
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

        //Capitura de tecla
        if(running)
            status = get_wch(&character);
    }

    //Se não tiver nada na memória, então a página cria uma nova
    if (!memory)
    {
        memory = (Persistence*)malloc(sizeof(Persistence));

        if (!memory)
            return result;
    }

    //Salva o que é necessário na memória 
    memory->list_select = list_context.element_in_focus;
    if (search_bar_context.element_in_focus)
        memory->is_search_bar_selected = 1;
    else
        memory->is_search_bar_selected = 0;

    //Passa o endereço da memória para o persistence
    *persistence = memory;

    return result;
}