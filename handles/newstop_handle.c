/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include <newstop_handle.h>
#include <newstop_page.h>
#include <stdlib.h>
#include <letter.h>

#define DBL 256

HandleResult init_newstop_handle(Style const *style, SimpleLinkedListNode *line_node, DoubleLinkedListNode *prev_node)
{
    void *persistence = NULL;

    int page_state = 0;
    int throw_popup = 0;

    PageArgs args = {
        .style = style,
        .persistence = &persistence,
        .state = 0,
        .throw_popup = 0
    };

    BusLine *line = (BusLine*)line_node->info;

    wchar_t error[256] = L"\0";

    int running = 1;
    HandleResult handle_results = {
        .state = state_stops,
        .first_value = line_node
    };

    while(running)
    {
        wchar_t title_buffer[DBL];
        
        if (prev_node && prev_node->info) {
            BusStop *prev_stop = (BusStop*)prev_node->info;
            swprintf(title_buffer, DBL, L"Adicionar após: %ls", prev_stop->name);
        } else 
            swprintf(title_buffer, DBL, L"Adicionar Primeira Parada");

        PageResult result = init_newstop_page(args, error, title_buffer);
        args.throw_popup = 0; 
        error[0] = L'\0';

        switch (result.action)
        {
        case page_action_back:
            handle_results.state = state_stops;
            running = 0;
            break;

        case page_action_text:
            // Variáveis temporárias
            BusStop temp_stop;
            Hours arrival;
            Hours departure;

            if (is_emptyw(result.first_text) || wcslen(result.first_text) > 20 ||
                is_emptyw(result.second_text) || wcslen(result.second_text) > 20 ||
                is_emptyw(result.third_text) || wcslen(result.third_text) > 20 ||
                !string_to_time(result.second_text, &arrival) ||   // Chegada
                !string_to_time(result.third_text, &departure)     // Saída
            )
            {
                args.throw_popup = 1;
                wcscpy(error, L"Dados inválidos! Use HHhMMm ou nome o é grande demais");
                break;
            }

            BusStop *new_stop = (BusStop*) malloc (sizeof(BusStop));
            if (!new_stop)
            {
                args.throw_popup = 1;
                wcscpy(error, L"Erro interno de memória");
                break;
            }

            // Preenche a struct
            wcscpy(new_stop->name, result.first_text);
            new_stop->arrival_time = arrival;
            new_stop->departure_time = departure;

            if (!insertStopAfter(line, prev_node, new_stop)) 
            {
                free(new_stop); // Importante liberar se falhar a inserção
                args.throw_popup = 1;
                wcscpy(error, L"Falha ao inserir na lista");
                break;
            }
            handle_results.state = state_stops; 
            running = 0;
            
            free(persistence);
            persistence = NULL;
            break;

        case page_action_fail:
            break;

        default:
            break;
        }
    }

    if(persistence) free(persistence);

    return handle_results;
}