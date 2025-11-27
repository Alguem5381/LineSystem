/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <editstop_page.h>
#include <draw.h>
#include <stdlib.h>
#include <letter.h>
#include <keys.h>

#define DBL 256

typedef struct Persistence
{
    wchar_t first_text[DBL];
    wchar_t second_text[DBL];
    wchar_t third_text[DBL];
    int selected;
} Persistence;

PageResult init_editstop_page(PageArgs args, wchar_t const *error, wchar_t *current_name, wchar_t *current_arr, wchar_t *current_dep)
{
    PageResult result = {0};

    // --- Contextos (Iguais ao newstop) ---
    DrawContext general_context = { .width = 100, .height = 100 };
    DrawContext first_text_context = { .width = 70, .height = 100, .element_in_focus = 1 };
    DrawContext second_text_context = { .width = 70, .height = 100 };
    DrawContext third_text_context = { .width = 70, .height = 100 };
    DrawContext dialog_context = { .width = 70, .height = 100 };

    set_style(args.style, &general_context);
    set_style(args.style, &first_text_context);
    set_style(args.style, &second_text_context);
    set_style(args.style, &third_text_context);
    set_style(args.style, &dialog_context);

    // Variáveis de Texto
    wchar_t first_text[DBL] = L"\0";
    wchar_t second_text[DBL] = L"\0";
    wchar_t third_text[DBL] = L"\0";
    
    // Título
    wchar_t title_text[DBL] = L"Editar Parada";

    // Teclas e Opções
    wchar_t *keys[] = {L"Esc", L"↑", L"←", L"→", L"↵"};
    wchar_t *options[] = {L"Cancelar", L"Sobe", L"Esq", L"Dir", L"Salvar"};
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    Persistence *memory = NULL;

    if (*args.persistence)
    {
        memory = (Persistence*)*args.persistence;

        wcscpy(first_text, memory->first_text);
        wcscpy(second_text, memory->second_text);
        wcscpy(third_text, memory->third_text);

        if (memory->selected == 1) {
            first_text_context.element_in_focus = 0;
            second_text_context.element_in_focus = 1;
            third_text_context.element_in_focus = 0;
        } else if (memory->selected == 2) {
            first_text_context.element_in_focus = 0;
            second_text_context.element_in_focus = 0;
            third_text_context.element_in_focus = 1;
        }
    }
    else
    {
        if (current_name) wcscpy(first_text, current_name);
        if (current_arr)  wcscpy(second_text, current_arr);
        if (current_dep)  wcscpy(third_text, current_dep);
    }

    // Variáveis de loop
    Key key = unknown;
    wint_t character;
    int need_draw = 1;
    int need_split = 1;
    int running = 1;
    int is_popup_on = args.throw_popup;

    while(running)
    {
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
            if (is_popup_on) {
                is_popup_on = 0;
                need_draw = 1;
                break;
            }
            wcscpy(result.first_text, first_text);
            wcscpy(result.second_text, second_text);
            wcscpy(result.third_text, third_text);
            result.action = page_action_text;
            running = 0;
            break;

        case left:
            if (is_popup_on) break;
            first_text_context.element_in_focus = 0;
            second_text_context.element_in_focus = 1;
            third_text_context.element_in_focus = 0;
            need_draw = 1;
            break;

        case right:
            if (is_popup_on) break;
            first_text_context.element_in_focus = 0;
            second_text_context.element_in_focus = 0;
            third_text_context.element_in_focus = 1;
            need_draw = 1;
            break;

        case up:
            if (is_popup_on) break;
            first_text_context.element_in_focus = 1;
            second_text_context.element_in_focus = 0;
            third_text_context.element_in_focus = 0;
            need_draw = 1;
            break;

        case common:
        case number:
            if (is_popup_on) break;
            if (first_text_context.element_in_focus)
                add_lastw(first_text, DBL, character);
            else if (second_text_context.element_in_focus)
                add_lastw(second_text, DBL, character);
            else if (third_text_context.element_in_focus)
                add_lastw(third_text, DBL, character);
            need_draw = 1;
            break;

        case backspace:
            if (is_popup_on) break;
            if (first_text_context.element_in_focus)
                remove_lastw(first_text);
            else if (second_text_context.element_in_focus)
                remove_lastw(second_text);
            else if (third_text_context.element_in_focus)
                remove_lastw(third_text);
            need_draw = 1;
            break;
            
        default: break;
        }

        // Layout
        if (need_split)
        {
            general_context.startx = 0; general_context.starty = 0;
            general_context.endx = COLS; general_context.endy = LINES;

            DrawContext main_panel_context = general_context;
            DrawContext botton_panel_context;

            main_panel_context.starty += 5;
            main_panel_context.endy -= 5;

            split_context(&botton_panel_context, &main_panel_context, 50, SECOND, VERTICAL);
            split_context(&first_text_context, &main_panel_context, 50, FIRST, VERTICAL);
            split_context(&second_text_context, &botton_panel_context, 50, FIRST, HORIZONTAL);
            split_context(&third_text_context, &botton_panel_context, 50, SECOND, HORIZONTAL);
            split_context(&dialog_context, &main_panel_context, 100, FIRST, HORIZONTAL);

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int ok = 1;
            ok *= draw_base_page(title_text, &general_context);
            ok *= draw_footer(keys, options, keys_length, &general_context);

            ok *= draw_text_box(first_text, &first_text_context);
            ok *= draw_text_box(second_text, &second_text_context);
            ok *= draw_text_box(third_text, &third_text_context);

            if (is_popup_on)
                ok *= draw_message_dialog(error, &dialog_context);

            need_draw = 0;
            refresh();
        }

        if (running) {
            int status = get_wch(&character);
            key = get_key(character, status);
        }
    }

    // Salvar persistência
    if (!memory) {
        memory = (Persistence*)malloc(sizeof(Persistence));
        if (!memory) {
            result.action = page_action_fail;
            return result;
        }
    }

    if (first_text_context.element_in_focus) memory->selected = 0;
    else if (second_text_context.element_in_focus) memory->selected = 1;
    else memory->selected = 2;

    wcscpy(memory->first_text, first_text);
    wcscpy(memory->second_text, second_text);
    wcscpy(memory->third_text, third_text);

    *args.persistence = memory;
    return result;
}