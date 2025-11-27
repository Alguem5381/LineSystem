/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include <line_handle.h>
#include <line_page.h>
#include <stdlib.h>
#include <letter.h>
#include <string.h>

#define DBL 256

HandleResult init_line_handle(Style const *style, Object *data)
{
    void *persistence = NULL;

    wchar_t **strings = NULL;
    SimpleLinkedListNode **elements = NULL;
    int elements_length = 0;

    get_lines_to_array(data, &elements, &elements_length, NULL);
    strings = create_line_strings(elements, elements_length);

    // Variável para manter o texto da busca atual entre chamadas
    wchar_t current_search[DBL] = {0};

    int page_state = 0;
    int throw_popup = 0;

    PageArgs args = {
        .style = style,
        .persistence = &persistence,
        .state = 0,
        .throw_popup = 0
    };

    int running = 1;
    HandleResult handle_result = {
        .state = state_exit,
        .first_value = NULL
    };

    while (running)
    {
        // Passa o popup se necessário
        args.throw_popup = throw_popup;
        
        PageResult result = init_line_page(args, strings, elements_length);

        // Reseta flags
        throw_popup = 0; 
        args.throw_popup = 0;

        switch (result.action)
        {
        case page_action_back:
            running = 0;
            handle_result.state = state_main;
            break;

        case page_action_text:
            // Salva o termo pesquisado
            wcscpy(current_search, result.first_text);

            // Libera memória antiga
            free_string_array(strings, elements_length);
            free(elements);
            elements = NULL;

            // Busca filtrada
            get_lines_to_array(data, &elements, &elements_length, current_search);
            strings = create_line_strings(elements, elements_length);
            break;

        case page_action_text_and_selected:
            if (!wcscmp(result.first_text, L"create"))
            {
                handle_result.state = state_new_line;
                running = 0;
                break;
            }

            if (!wcscmp(result.first_text, L"delete"))
            {
                if (result.selected_index >= 0 && result.selected_index < elements_length) 
                {
                    // Pega o nó direto do vetor de elementos
                    SimpleLinkedListNode *node_to_delete = elements[result.selected_index];

                    removeLineNode(data, node_to_delete);

                    free_string_array(strings, elements_length);
                    free(elements);
                    elements = NULL;
                    get_lines_to_array(data, &elements, &elements_length, current_search);
                    strings = create_line_strings(elements, elements_length);
                }
                break;
            }
            break;

        case page_action_select:
            if (result.selected_index >= 0 && result.selected_index < elements_length) {
                handle_result.first_value = elements[result.selected_index];
                handle_result.state = state_stops;
                running = 0;
            }
            break;

        default:
            break;
        }
    }

    if (persistence) free(persistence);
    
    free_string_array(strings, elements_length);
    free(elements);

    return handle_result;
}