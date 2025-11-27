/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <line_page.h>
#include <draw.h>
#include <stdlib.h>
#include <letter.h>
#include <keys.h>

// (D)Default (B)Buffer (L) Length
#define DBL 256

// Struct para memória persistente
typedef struct Persistence
{
    wchar_t search_text[DBL];
    int select_index;
} Persistence;

// Inicializador da página

PageResult init_line_page(PageArgs args, wchar_t *elements[], int elements_length)
{
    PageResult result = {0};

    //Contextos
    DrawContext general_context = {
        .width = 100,
        .height = 100
    };
    DrawContext list_context = {
        .height = 100,
        .width = 70,
        .element_in_focus = 0
    };
    DrawContext seach_bar_context = {
        .height = 100,
        .width = 70
    };
    DrawContext dialog_context = {
        .height= 100,
        .width = 50,
        .element_in_focus = 0
    };

    //Definição de estilo para os contextos
    set_style(args.style, &general_context);
    set_style(args.style, &list_context);
    set_style(args.style, &seach_bar_context);
    set_style(args.style, &dialog_context);

    //Vetores utilizados
    wchar_t defaul_search_bar_text[DBL] = L"Digite a linha desejada";
    wchar_t search_bar_text[DBL] = {L"\0"};

    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵", L"←", L"→"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Selecionar", L"Criar", L"Deletar"};

    wchar_t *keys_popup[] = {L"Enter", L"←", L"→"};
    wchar_t *options_popup[] = {L"Confirmar", L"Direita", L"Esquerda"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);
    int keys_popup_length = sizeof(keys_popup) / sizeof(keys_popup[0]);

    // Recuperando memória persistente
    Persistence *memory = NULL;

    if (*args.persistence)
    {
        memory = (Persistence*)*args.persistence;

        if (!is_emptyw(memory->search_text))
            wcscpy(search_bar_text, memory->search_text);

        if (memory->select_index && memory->select_index < elements_length)
            list_context.element_in_focus = memory->select_index;
    }

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
            if (is_popup_on) break;

            result.action = page_action_back;
            running = 0;
            break;

        case enter:
            if (is_popup_on)
            {
                is_popup_on = 0;

                if (!dialog_context.element_in_focus)
                {
                    need_draw = 1;
                    break;
                }

                wcscpy(result.first_text, L"delete");
                result.selected_index = list_context.element_in_focus;

                result.action = page_action_text_and_selected;
                running = 0;
                break;
            }

            result.selected_index = list_context.element_in_focus;
            result.action = page_action_select;
            running = 0;
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

        case left:
            if (is_popup_on)
            {
                dialog_context.element_in_focus = 1;
                need_draw = 1;
                break;
            }

            wcscpy(result.first_text, L"create");
            result.action = page_action_text_and_selected;
            result.selected_index = 0;
            running = 0;
            break;

        case right:
            if (is_popup_on)
            {
                dialog_context.element_in_focus = 0;
                need_draw = 1;
                break;
            }

            is_popup_on = 1;
            need_draw = 1;
            break;

        case number:
            if (is_popup_on) break;

            add_lastw(search_bar_text, DBL, character);
            wcscpy(result.first_text, search_bar_text);
            result.action = page_action_text;

            need_draw = 1;
            running = 0;

            break;

        case backspace:
            if (is_popup_on) break;

            remove_lastw(search_bar_text);
            wcscpy(result.first_text, search_bar_text);
            result.action = page_action_text;

            need_draw = 1;
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

            DrawContext main_panel_context = general_context;

            main_panel_context.starty += 5;
            main_panel_context.endy -= 5;

            split_context(&seach_bar_context, &main_panel_context, 30, FIRST, VERTICAL);
            split_context(&list_context, &main_panel_context, 20, SECOND, VERTICAL);
            split_context(&dialog_context, &main_panel_context, 100, FIRST, HORIZONTAL);

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int sucessful = 1;

            sucessful *= draw_base_page(L"Linhas", &general_context);
            sucessful *= draw_list(elements, elements_length, &list_context);
            if (is_emptyw(search_bar_text))
                sucessful *= draw_text_box(defaul_search_bar_text, &seach_bar_context);
            else
                sucessful *= draw_text_box(search_bar_text, &seach_bar_context);

            if (is_popup_on)
            {
                sucessful *= draw_yes_or_no_dialog(L"Tem certeza que deseja excluir", &dialog_context);
                sucessful *= draw_footer(keys_popup, options_popup, keys_popup_length, &general_context);
            }
            else
                sucessful *= draw_footer(keys, options, keys_length, &general_context);

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
        memory = (Persistence*)malloc(sizeof(Persistence));

        if (!memory)
        {
            result.action = page_action_fail;
            return result;
        }
    }

    // Salva o que é necessário na memória
    memory->select_index = list_context.element_in_focus;
    wcscpy(memory->search_text, search_bar_text);

    // Modifica o ponteiro no handle
    *args.persistence = memory;

    return result;
}