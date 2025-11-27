#include <stop_handle.h>
#include <stop_page.h>
#include <stdlib.h>
#include <letter.h>

#define DBL 256

HandleResult init_stop_handle(Style const *style, SimpleLinkedListNode *line_node)
{
    void *persistence = NULL;

    wchar_t **strings = NULL;
    DoubleLinkedListNode **elements = NULL;
    int elements_length = 0;

    get_stops_to_array(line_node, &elements, &elements_length, NULL);
    strings = create_stop_strings(elements, elements_length); 

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
        .first_value = line_node, // Mantém a linha selecionada para o próximo estado
        .second_value = NULL,
        .third_value = NULL
    };

    // Cast seguro para usar o nome da linha no título da página
    BusLine *current_bus_line = (BusLine*)line_node->info;

    while (running)
    {
        PageResult result = init_stop_page(args, strings, elements_length, current_bus_line->name);
        throw_popup = 0;

        switch (result.action)
        {
        case page_action_back:
            running = 0;
            handle_result.state = state_line;
            break;

        case page_action_text:
            wcscpy(current_search, result.first_text);

            free_string_array(strings, elements_length);
            free(elements);
            elements = NULL;

            get_stops_to_array(line_node, &elements, &elements_length, current_search);
            strings = create_stop_strings(elements, elements_length);
            break;

        case page_action_text_and_selected:
            if (!wcscmp(result.first_text, L"create"))
            {
                if (elements && elements_length > 0)
                    handle_result.third_value = elements[result.selected_index];
                else
                    handle_result.third_value = NULL;

                handle_result.state = state_new_stop;
                running = 0;
                break;
            }

            if (!wcscmp(result.first_text, L"delete"))
            {
                // CORREÇÃO 2: Passar a Struct BusLine*, não o Nó da lista
                if (elements && result.selected_index < elements_length) {
                    removeStopNode(current_bus_line, elements[result.selected_index]);
                }

                // Recarrega
                free(elements);
                elements = NULL;
                free_string_array(strings, elements_length);
                
                get_stops_to_array(line_node, &elements, &elements_length, current_search);
                strings = create_stop_strings(elements, elements_length);
                break;
            }
            break;

        case page_action_select:
            handle_result.second_value = elements[result.selected_index];
            handle_result.state = state_edit_stop;
            running = 0;
            break;

        default:
            break;
        }
    }

    free(persistence);
    free(elements);
    free_string_array(strings, elements_length);

    return handle_result;
}