/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include <style.h>

void set_style(Style const *style, DrawContext *general_context)
{
    general_context->default_border_color = style->default_border_color;
    general_context->default_pair_color = style->default_pair_color;
    general_context->divider_color = style->divider_color;
    general_context->on_focus_border_color = style->on_focus_border_color;
    general_context->on_focus_pair_color = style->on_focus_pair_color;
}