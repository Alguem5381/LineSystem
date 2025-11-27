#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <login_page.h>
#include <draw.h>
#include <stdlib.h>
#include <letter.h>
#include <keys.h>

// (D)Default (B)Buffer (L) Length
#define DBL 256

// Struct para memória persistente
typedef struct Persistence
{
    wchar_t login_bar_text[DBL];
} Persistence;

// Inicializador da página

PageResult init_login_page(PageArgs args)
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
    DrawContext login_bar_context = 
    {
        .width = 50,
        .height = 100,
        .element_in_focus = 1
    };
    DrawContext dialog_context =
    {
        .width = 50,
        .height = 100
    };

    //Definição de estilo para os contextos
    set_style(args.style, &general_context);
    set_style(args.style, &login_bar_context);
    set_style(args.style, &dialog_context);
    
    //Vetores utilizados
    wchar_t default_login_bar_text[] = L"Digite a senha para entrar";
    wchar_t login_bar_text[DBL] = L"\0";

    wchar_t *keys[] = {L"Esc", L"↵"};
    wchar_t *options[] = {L"Sair", L"Confirmar"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    // Recuperando memória persistente
    Persistence *memory = NULL;

    if (*args.persistence)
    {
        memory = (Persistence*)*args.persistence;

        if(!is_emptyw(memory->login_bar_text))
            wcscpy(login_bar_text, memory->login_bar_text);
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

            wcscpy(result.first_text, login_bar_text);

            result.action = page_action_text;
            running = 0;
            break;

        case common:
        case number:
            if (is_popup_on) break;

            add_lastw(login_bar_text, DBL, character);
            need_draw = 1;
            break;

        case backspace:
            if (is_popup_on) break;

            remove_lastw(login_bar_text);
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

            split_context(&login_bar_context, &main_panel_context, 100, FIRST, HORIZONTAL);
            split_context(&dialog_context, &main_panel_context, 100, FIRST, HORIZONTAL);

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int sucessful = 1;

            sucessful *= draw_base_page(L"Manutenção", &general_context);
            sucessful *= draw_footer(keys, options, keys_length, &general_context);

            if (is_emptyw(login_bar_text))
                sucessful *= draw_text_box(default_login_bar_text, &login_bar_context);
            else
                sucessful *= draw_text_box(login_bar_text, &login_bar_context);

            if (is_popup_on)
                sucessful *= draw_message_dialog(L"Senha incorreta", &dialog_context);

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

    wcscpy(memory->login_bar_text, login_bar_text);

    // Modifica o ponteiro no handle
    *args.persistence = memory;

    return result;
}