/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include <template_handle.h>
#include <template_page.h>

States init_template_handle(Style const *style)
{
    void *persistence = NULL;

    //Estado da página
    int page_state = 0;
    int throw_popup = 0;

    PageArgs args =
    {
        .style = style,
        .persistence = &persistence,
        .state = 0,
        .throw_popup = 0
    };

    //Loop principal
    int running = 1;
    States state;

    while(running)
    {
        PageResult result = init_template_page(args);
        throw_popup = 0;

        //Manipulação do resultado da página
        switch (result.action)
        {
        //Caso seja uma ação de voltar
        case page_action_back:
            //Faz algo
            break;

        //Caso seja um texto
        case page_action_text:
            //Faz algo
            break;

        //Caso seja um texto e um índice
        case page_action_text_and_selected:
            //Faz algo
            break;

        case page_action_select:
            //Faz algo
            break;

        case page_action_fail:
            //Faz algo
            break;

        default:
            break;
        }
    }

    free(persistence);

    return state;
}