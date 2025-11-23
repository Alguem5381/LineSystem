#ifndef DRAW_H
#define DRAW_H

//Para facilitar o uso da função de dividir contextos
#define HORIZONTAL 0
#define VERTICAL 1

#define FIRST 0
#define SECOND 1

#include <wchar.h>

typedef struct DrawContext
{
    int default_pair_color;                                             //Chave de cor para fundo e texto
    int divider_color;                                                  //Chave de cor para divisor
    int default_border_color;                                           //Chave de cor de borda padrão 

    int on_focus_border_color;                                          //Chave de cor da borda quando em foco
    int on_focus_pair_color;                                            //Chave de cor para fundo e texto quando em foco
    int element_in_focus;                                               //Numero do elemento em foco pelo cursor 

    int width, height;                                                  //Porcentagem minima de ocupação do objeto na área de desenho
    int startx, starty;                                                 //Valor de início da área de desenho
    int endx, endy;                                                     //Valor de fim da área de desenho
} DrawContext;

int draw_text_box(wchar_t const *text, DrawContext const *context);

int draw_base_page(wchar_t const *title, DrawContext const *context);

int draw_message_dialog(wchar_t const *message, DrawContext const *context);

int draw_yes_or_no_dialog(wchar_t const *message, DrawContext const *context);

int draw_footer(wchar_t **keys, wchar_t **options, int array_length, DrawContext const *context);

int draw_list(wchar_t **elements, int array_length, DrawContext const *context);

int draw_label(wchar_t const *text, DrawContext const *context);

int split_context(DrawContext *dest, DrawContext const *src, int percent, int half, int direction);

#endif //DRAW_H