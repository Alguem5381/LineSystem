#include <login_handle.h>
#include <login_page.h>
#include <stdlib.h>

int is_valid_password(wchar_t *password)
{
    if(!wcscmp(L"Ada lovelace", password))
        return 1;
    return 0;
}

HandleResult init_login_handle(Style const *style)
{
    void *persistence = NULL;

    //Estado da página
    int page_state = 0;

    PageArgs args =
    {
        .style = style,
        .persistence = &persistence,
        .state = 0,
        .throw_popup = 0
    };

    //Loop principal
    int running = 1;
    HandleResult handle_result =
    {
        .state = state_exit,
        .first_value = NULL
    };

    while(running)
    {
        PageResult result = init_login_page(args);
        args.throw_popup = 0;

        //Manipulação do resultado da página
        switch (result.action)
        {
        //Caso seja uma ação de voltar
        case page_action_back:
            running = 0;
            handle_result.state = state_main;
            break;

        //Caso seja um texto
        case page_action_text:
            if (!is_valid_password(result.first_text))
                args.throw_popup = 1;
            else
            {
                handle_result.state = state_line;
                running = 0;
            }
            break;

        default:
            break;
        }
    }

    free(persistence);

    return handle_result;
}