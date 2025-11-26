#include <newline_handle.h>
#include <newline_page.h>
#include <stdlib.h>
#include <letter.h>

#define DBL 256

HandleResult init_newline_handle(Style const *style)
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

    wchar_t error[256] = L"\0";

    //Loop principal
    int running = 1;
    HandleResult handle_results =
    {
        .state = state_line,
        .first_value = NULL
    };

    while(running)
    {
        PageResult result = init_newline_page(args, error);
        throw_popup = 0;

        //Manipulação do resultado da página
        switch (result.action)
        {
        //Caso seja uma ação de voltar
        case page_action_back:
            handle_results.state = state_line;
            running = 0;
            break;

        //Caso seja um texto
        case page_action_text:
            //Tenta criar, se falhar colocar o texto de error em error e ativa um popup
            if (is_emptyw(result.first_text) || is_emptyw(result.second_text) || 1/*func aqui*/ )
            {
                args.throw_popup = 1;
                wcscpy(error, L"Falha ao criar");
                break;
            }

            args.throw_popup = 1;
            wcscpy(error, L"Criado com sucesso");
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