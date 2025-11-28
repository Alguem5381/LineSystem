/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef DRAW_H
#define DRAW_H

/// @defgroup Draw
/// @brief Estrutura e funções que abstraem o ncurses
/// @{

//Para facilitar o uso da função de dividir contextos
#define HORIZONTAL 0
//Para facilitar o uso da função de dividir contextos
#define VERTICAL 1

//Para facilitar o uso da função de dividir contextos
#define FIRST 0
//Para facilitar o uso da função de dividir contextos
#define SECOND 1

#include <wchar.h>

/// @brief Estrutura para guardar informações de como as funções de desenho devem desenhar na tela
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

/// @brief Desenha uma caixa de texto 
/// @param text O texto na caixa
/// @return 1 se sucesso e 0 se falha
int draw_text_box(wchar_t const *text, DrawContext const *context);

/// @brief Desenha uma base de página com titulo 
/// @param title O titulo
/// @return 1 se sucesso e 0 se falha
int draw_base_page(wchar_t const *title, DrawContext const *context);

/// @brief Desenha uma janela de diálogo com uma mensagem 
/// @param message A mensagem
/// @return 1 se sucesso e 0 se falha
int draw_message_dialog(wchar_t const *message, DrawContext const *context);

/// @brief Desenha uma janela de diálogo com opções de sim ou não
/// @param message A mensagem
/// @return 1 se sucesso e 0 se falha
int draw_yes_or_no_dialog(wchar_t const *message, DrawContext const *context);

/// @brief Desenha um rodapé com as teclas e suas funcionalidades
/// @param keys As teclas
/// @param options As funcionalidade
/// @param array_length O tamanho dos dois vetores
/// @return 1 se sucesso e 0 se falha
int draw_footer(wchar_t **keys, wchar_t **options, int array_length, DrawContext const *context);

/// @brief Desenha uma lista
/// @param elements Os elementos da lista
/// @param array_length O tamanho da lista
/// @return 1 se sucesso e 0 se falha
int draw_list(wchar_t **elements, int array_length, DrawContext const *context);

/// @brief Desenha uma label de texto dinâmico
/// @param text O texto
/// @return 1 se sucesso e 0 se falha
int draw_label(wchar_t const *text, DrawContext const *context);

/// @brief Divide as coordenadas de um contexto src e coloca elas em um contexto dest
/// @param dest O destino
/// @param src A fonte
/// @param percent A porcentagem do quanto deve ser divido
/// @param half O pedaço que o contexto deve receber da divisão
/// @param direction A direção de como ele deve ser dividido
/// @return 1 se sucesso e 0 se falha
int split_context(DrawContext *dest, DrawContext const *src, int percent, int half, int direction);

/// @} Fim do grupo

#endif //DRAW_H