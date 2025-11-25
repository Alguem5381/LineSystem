#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <template_page.h>
#include <draw.h>
#include <stdlib.h>
#include <letter.h>
#include <keys.h>

// (D)Default (B)Buffer (L) Length
#define DBL 256

// Struct para memória persistente
typedef struct Persistence
{
    int some_value;
} Persistence;

// Inicializador da página

PageResult init_template_page(PageArgs args)
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
    set_style(args.style, &general_context);

    //Vetores utilizados
    wchar_t *keys[] = {L"Esc", L"↑", L"↓", L"↵"};
    wchar_t *options[] = {L"Sair", L"Subir", L"Descer", L"Selecionar"};

    //Tamanho dos vetores
    int keys_length = sizeof(keys) / sizeof(keys[0]);

    // Recuperando memória persistente
    Persistence *memory = NULL;

    if (*args.persistence)
    {
        memory = (Persistence*)*args.persistence;

        //Usa a memoria
    }

    // Aplicando estado
    switch (args.state)
    {
    case 0:
        // Faz algo
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

            need_split = 0;
        }

        // Desenho
        if (need_draw)
        {
            int sucessful = 1;

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
    memory->some_value = 5;

    // Modifica o ponteiro no handle
    *args.persistence = memory;

    return result;
}