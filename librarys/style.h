/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef STYLE_H
#define STYLE_H

#include <draw.h>

/// @brief Estrutura para guardar os numeros dos pares de cor
typedef struct Style
{
    int default_pair_color;                                             //Chave de cor para fundo e texto
    int divider_color;                                                  //Chave de cor para divisor
    int default_border_color;                                           //Chave de cor de borda padrão 

    int on_focus_border_color;                                          //Chave de cor da borda quando em foco
    int on_focus_pair_color;                                            //Chave de cor para fundo e texto quando em foco
} Style;

/// @brief Função simples para aplicar os estilos nos contextos
/// @param style A struct do estilo
/// @param general_context O contexto
void set_style(Style const *style, DrawContext *general_context);

#endif //STYLE_H