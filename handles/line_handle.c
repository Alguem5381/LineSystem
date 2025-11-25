#include <line_handle.h>
#include <line_page.h>
#include <stdlib.h>

#define DBL 256

HandleResult init_line_handle(Style const *style)
{
    void *persistence = NULL;

    wchar_t *elements[] = {L"Nada ainda1", L"Nada ainda2", L"Nada ainda3"};
    int elements_length = sizeof(elements) / sizeof(elements[0]);

    // Estado da página
    int page_state = 0;
    int throw_popup = 0;

    PageArgs args =
        {
            .style = style,
            .persistence = &persistence,
            .state = 0,
            .throw_popup = 0};

    // Loop principal
    int running = 1;
    HandleResult handle_result;

    while (running)
    {
        PageResult result = init_line_page(args, elements, elements_length);
        throw_popup = 0;

        // Manipulação do resultado da página
        switch (result.action)
        {
        // Caso seja uma ação de voltar
        case page_action_back:
            running = 0;
            handle_result.state = state_main;
            break;

        // Caso seja um texto
        case page_action_text:
            // Faz algo
            break;

        // Caso seja um texto e um índice
        case page_action_text_and_selected:
            if (!wcscmp(result.text, L"create"))
            {
                free(handle_result.value);
                handle_result.value = (wchar_t *)malloc(sizeof(wchar_t) * DBL);

                if (!handle_result.value)
                {
                    handle_result.state = state_exit;
                    running = 0;
                    break;
                }

                wcscpy(handle_result.value, elements[result.selected_index]);
                handle_result.state = state_new_line;
                running = 0;
                break;
            }

            if (!wcscmp(result.text, L"delete"))
            {
                //deleta
                //cria nova lista
                break;
            }
            break;

        case page_action_select:
            free(handle_result.value);
            handle_result.value = (wchar_t *)malloc(sizeof(wchar_t) * DBL);

            if (!handle_result.value)
            {
                handle_result.state = state_exit;
                running = 0;
                break;
            }

            wcscpy(handle_result.value, elements[result.selected_index]);
            handle_result.state = state_stops;
            running = 0;

            break;

        case page_action_fail:
            // Faz algo
            break;

        default:
            break;
        }
    }

    free(persistence);

    return handle_result;
}