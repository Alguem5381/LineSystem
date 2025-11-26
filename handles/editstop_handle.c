#include <editstop_handle.h>
#include <editstop_page.h>
#include <stdlib.h>
#include <letter.h>

#define DBL 256
#define TIME_DBL 7

HandleResult init_editstop_handle(Style const *style, wchar_t *line, wchar_t *stop)
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

    wchar_t first_time[TIME_DBL];
    wchar_t second_time[TIME_DBL];

    //get_stop(object, stop, );

    wchar_t error[256] = L"\0";

    //Loop principal
    int running = 1;
    HandleResult handle_results =
    {
        .state = state_stops,
        .first_value = line
    };

    while(running)
    {
        PageResult result = init_editstop_page(args, error, stop, first_time, second_time);
        throw_popup = 0;

        //Manipulação do resultado da página
        switch (result.action)
        {
        //Caso seja uma ação de voltar
        case page_action_back:
            handle_results.state = state_stops;
            running = 0;
            break;

        //Caso seja um texto
        case page_action_text:
            //Tenta criar, se falhar colocar o texto de error em error e ativa um popup
            if (is_emptyw(result.first_text) || is_emptyw(result.second_text) || is_emptyw(result.third_text) || 1/*func aqui*/)
            {
                args.throw_popup = 1;
                wcscpy(error, L"Falha ao editar");
                break;
            }

            args.throw_popup = 1;
            wcscpy(error, L"Editado com sucesso");
            free(persistence);
            persistence = NULL;
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

    return handle_results;
}