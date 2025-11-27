/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <newline_page.h>
#include <draw.h>
#include <stdlib.h>
#include <letter.h>
#include <keys.h>

// (D)Default (B)Buffer (L) Length
#define DBL 256

// Struct para memória persistente
typedef struct Persistence
{
    wchar_t first_text[DBL];
    wchar_t second_text[DBL];
    int selected;
} Persistence;

// Inicializador da página

PageResult init_newline_page(PageArgs args, wchar_t const *error)
{
    PageResult result = {0};

    //Contextos
    DrawContext general_context =
    {
        .width = 100,
        .height = 100
    };
    DrawContext first_text_context =
    {
        .width = 70,
        .height = 100,
        .element_in_focus = 1
    };
    DrawContext second_text_context =
    {
        .width = 70,
        .height = 100
    };
    DrawContext dialog_context =
    {
        .width = 70,
        .height = 100
    };

    //Definição de estilo para os contextos
    set_style(args.style, &general_context);
    set_style(args.style, &first_text_context);
    set_style(args.style, &second_text_context);
    set_style(args.style, &dialog_context);

    //Vetores utilizados
    wchar_t default_first_text[DBL] = L"Digite o número da linha";
    wchar_t default_second_text[DBL] = L"Digite o nome da companhia";
    wchar_t first_text[DBL] = L"\0";
    wchar_t second_text[DBL] = L"\0";

    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵"};
    wchar_t *options[] = {L"Sair", L"Sobe", L"Desce", L"Confirmar"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    // Recuperando memória persistente
    Persistence *memory = NULL;

    if (*args.persistence)
    {
        memory = (Persistence*)*args.persistence;

        if (!is_emptyw(memory->first_text))
            wcscpy(first_text, memory->first_text);
        if (!is_emptyw(memory->second_text))
            wcscpy(second_text, memory->second_text);
        if (memory->selected)
        {
            first_text_context.element_in_focus = 0;
            second_text_context.element_in_focus = 1;
        }
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
                need_draw = 1;
                break;
            }
            wcscpy(result.first_text, first_text);
            wcscpy(result.second_text, second_text);
            result.action = page_action_text;
            running = 0;
            break;

        case down:
            if (is_popup_on) break;

            first_text_context.element_in_focus = 0;
            second_text_context.element_in_focus = 1;
            need_draw = 1;
            break;

        case up:
            if (is_popup_on) break;

            first_text_context.element_in_focus = 1;
            second_text_context.element_in_focus = 0;
            need_draw = 1;
            break;

        case common:
            if (is_popup_on) break;
            if (second_text_context.element_in_focus)
            {
                add_lastw(second_text, DBL, character);
                need_draw = 1;
            }
            break;

        case number:
            if (is_popup_on) break;
            if (first_text_context.element_in_focus)
                add_lastw(first_text, DBL, character);
            else if (second_text_context.element_in_focus)
                add_lastw(second_text, DBL, character);
            need_draw = 1;
            break;

        case backspace:
            if (first_text_context.element_in_focus)
                remove_lastw(first_text);
            else if (second_text_context.element_in_focus)
                remove_lastw(second_text);
            need_draw = 1;
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

            split_context(&first_text_context, &main_panel_context, 50, FIRST, VERTICAL);
            split_context(&second_text_context, &main_panel_context, 50, SECOND, VERTICAL);
            split_context(&dialog_context, &main_panel_context, 100, FIRST, HORIZONTAL);

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int sucessful = 1;

            sucessful *= draw_base_page(L"Criando linha", &general_context);
            sucessful *= draw_footer(keys, options, keys_length, &general_context);

            if (is_emptyw(first_text))
                sucessful *= draw_text_box(default_first_text, &first_text_context);
            else
                sucessful *= draw_text_box(first_text, &first_text_context);

            if (is_emptyw(second_text))
                sucessful *= draw_text_box(default_second_text, &second_text_context);
            else
                sucessful *= draw_text_box(second_text, &second_text_context);

            if (is_popup_on)
                sucessful *= draw_message_dialog(error, &dialog_context);

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
    if (first_text_context.element_in_focus)
        memory->selected = 0;
    else
        memory->selected = 1;

    wcscpy(memory->first_text, first_text);
    wcscpy(memory->second_text, second_text);

    // Modifica o ponteiro no handle
    *args.persistence = memory;

    return result;
}