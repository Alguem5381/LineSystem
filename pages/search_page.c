#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <search_page.h>
#include <draw.h>
#include <stdlib.h>
#include <letter.h>
#include <keys.h>

//(D)Default (B)Buffer (L) Length
#define DBL 256

// Struct para memória persistente

typedef struct Persistence
{
    wchar_t search_text[DBL];
    wchar_t time_text[DBL];
    int list_select;
    int is_search_bar_selected;
} Persistence;

// Inicializador da página

PageResult init_search_page(Style const *style, void **persistence, int state, wchar_t const *label_text, wchar_t *elements[], int const elements_length)
{
    PageResult result = {0};

    // Contextos com definições padrões
    DrawContext general_context = {
        .width = 100,
        .height = 100};
    DrawContext list_context = {
        .height = 100,
        .width = 90};
    DrawContext search_bar_context = {
        .height = 100,
        .width = 90,
        .element_in_focus = 1};
    DrawContext time_bar_context = {
        .height = 100,
        .width = 90};
    DrawContext label_context = {
        .height = 100,
        .width = 90};
    DrawContext dialog_context = {
        .height = 100,
        .width = 50
    };

    // Definição de estilo para os contextos
    set_style(style, &general_context);
    set_style(style, &list_context);
    set_style(style, &search_bar_context);
    set_style(style, &time_bar_context);
    set_style(style, &label_context);
    set_style(style, &dialog_context);

    // Vetores utilizados
    wchar_t default_search_text[DBL];
    wchar_t default_time_text[DBL];
    wchar_t dialog_text[DBL] = L"Digite um horário válido";
    wchar_t title_text[DBL];
    wchar_t search_text[DBL] = L"\0";
    wchar_t time_text[DBL] = L"\0";

    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"←", L"→", L"↵"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Direita", L"Esquerda", L"Selecionar"};

    // Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    // Recuperando memória persistente
    Persistence *memory = NULL;

    if ((*persistence))
    {
        memory = (Persistence*)*persistence;

        // Proteção contra uma possível memória invalida
        if (memory->list_select < elements_length)
            list_context.element_in_focus = memory->list_select;
        else
            list_context.element_in_focus = 0;

        if (!memory->is_search_bar_selected)
        {
            search_bar_context.element_in_focus = 0;
            time_bar_context.element_in_focus = 1;
        }

        if (!is_emptyw(memory->search_text))
            wcscpy(search_text, memory->search_text);

        if (!is_emptyw(memory->time_text))
            wcscpy(time_text, memory->time_text);
    }

    // Aplicando estado
    switch (state)
    {
    case 0:
        wcscpy(title_text, L"Busca de rotas - Origem");
        wcscpy(default_search_text, L"Pesquise uma parada inicial");
        wcscpy(default_time_text, L"Hora de saída. (Ex: 11h30m)");
        break;

    case 1:
        wcscpy(title_text, L"Busca de rotas - Destino");
        wcscpy(default_search_text, L"Pesquise uma parada de destino");
        wcscpy(default_time_text, L"Hora de chegada. (Ex: 19h10m)");
        break;

    case 2:
        wcscpy(title_text, L"Busca de rotas - Resultado");
        wcscpy(default_search_text, L"Pesquise uma parada de destino");
        wcscpy(default_time_text, L"Hora de chegada. (Ex: 19h10m)");
        break;
    
    default:
        break;
    }

    // Para capitura teclas
    Key key = unknown;
    wint_t character;

    // Variaveis para o loop
    int need_draw = 1;
    int need_split = 1;
    int running = 1;
    int is_popup_on = 0;

    while (running)
    {
        // Tratamento de teclas
        switch (key)
        {
        case resize:
            resize_term(0, 0);
            need_split = 1;
            need_draw = 1;
            break;

        case right:
            if (is_popup_on) break;

            search_bar_context.element_in_focus = 0;
            time_bar_context.element_in_focus = 1;
            need_draw = 1;
            break;

        case left:
            if (is_popup_on) break;

            search_bar_context.element_in_focus = 1;
            time_bar_context.element_in_focus = 0;
            need_draw = 1;
            break;

        case up:
            if (is_popup_on) break;

            if (list_context.element_in_focus > 0)
            {
                list_context.element_in_focus--;
                need_draw = 1;
            }
            break;

        case down:
            if (is_popup_on) break;

            if (list_context.element_in_focus < elements_length - 1)
            {
                list_context.element_in_focus++;
                need_draw = 1;
            }
            break;

        case esc:
            if (is_popup_on) break;

            result.action = page_action_back;
            running = 0;
            break;

        case enter:
            if (is_popup_on)
            {
                is_popup_on = 0;
                need_draw = 1;
                break;
            }

            if (is_emptyw(time_text))
            {
                is_popup_on = 1;
                need_draw = 1;
                need_split = 1;
                break;
            }

            result.action = page_action_text_and_selected;
            wcscpy(result.text, time_text);
            result.selected_index = list_context.element_in_focus;
            running = 0;

            break;

        case common:
        case number:
            if (search_bar_context.element_in_focus)
            {
                add_lastw(search_text, DBL, character);
                result.action = page_action_text;
                wcscpy(result.text, search_text);
                running = 0;
            }
            else
                add_lastw(time_text, DBL, character);

            need_draw = 1;
            break;

        case backspace:
            if (search_bar_context.element_in_focus)
            {
                if (!remove_lastw(search_text))
                {
                    need_draw = 1;
                    break;
                }

                result.action = page_action_text;
                wcscpy(result.text, search_text);
                running = 0;
            }
            else
                remove_lastw(time_text);

            need_draw = 1;
            break;

        default:
            break;
        }

        // Definição das coordendas dos contextos
        if (need_split)
        {
            general_context.startx = 0;
            general_context.starty = 0;
            general_context.endx = COLS;
            general_context.endy = LINES;

            DrawContext main_panel_context = general_context;
            DrawContext left_panel_context;
            DrawContext right_panel_context;

            main_panel_context.starty += 5;
            main_panel_context.endy -= 5;

            split_context(&left_panel_context, &main_panel_context, 50, FIRST, HORIZONTAL);
            split_context(&right_panel_context, &main_panel_context, 50, SECOND, HORIZONTAL);

            if (is_popup_on)
            {
                split_context(&dialog_context, &main_panel_context, 100, FIRST, HORIZONTAL);
            }

            split_context(&list_context, &left_panel_context, 20, SECOND, VERTICAL);
            split_context(&search_bar_context, &left_panel_context, 30, FIRST, VERTICAL);
            split_context(&time_bar_context, &right_panel_context, 30, FIRST, VERTICAL);
            split_context(&label_context, &right_panel_context, 20, SECOND, VERTICAL);

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int sucessful = 1;
            sucessful *= draw_base_page(title_text, &general_context);
            sucessful *= draw_footer(keys, options, keys_length, &general_context);
            sucessful *= draw_label(label_text, &label_context);
            sucessful *= draw_list(elements, elements_length, &list_context);

            if (!is_emptyw(search_text))
                sucessful *= draw_text_box(search_text, &search_bar_context);
            else
                sucessful *= draw_text_box(default_search_text, &search_bar_context);

            if (!is_emptyw(time_text))
                sucessful *= draw_text_box(time_text, &time_bar_context);
            else
                sucessful *= draw_text_box(default_time_text, &time_bar_context);

            if (is_popup_on)
                sucessful *= draw_message_dialog(dialog_text, &dialog_context);

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

    // Se não tiver nada na memória, então a página cria uma nova
    if (!memory)
    {
        memory = (Persistence *)malloc(sizeof(Persistence));

        if (!memory)
        {
            result.action = page_action_fail;
            return result;
        }
    }

    // Salva o que é necessário na memória
    memory->list_select = list_context.element_in_focus;
    wcscpy(memory->search_text, search_text);
    wcscpy(memory->time_text, time_text);
    if (search_bar_context.element_in_focus)
        memory->is_search_bar_selected = 1;
    else
        memory->is_search_bar_selected = 0;

    // Modifica o ponteiro no handle
    *persistence = memory;

    return result;
}