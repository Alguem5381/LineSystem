#include <editstop_handle.h>
#include <editstop_page.h>
#include <stdlib.h>
#include <letter.h>
#include <stdio.h>

#define DBL 256

HandleResult init_editstop_handle(Style const *style, DoubleLinkedListNode *target_node, SimpleLinkedListNode *line_node)
{
    void *persistence = NULL;

    // Validação de segurança
    if (!target_node || !target_node->info) {
        HandleResult error_result = { .state = state_stops };
        return error_result;
    }

    BusStop *current_stop = (BusStop*)target_node->info;

    // Prepara os textos iniciais (converte Horas para String)
    wchar_t init_arrival[DBL];
    wchar_t init_departure[DBL];

    swprintf(init_arrival, DBL, L"%02dh%02dm", current_stop->arrival_time.hours, current_stop->arrival_time.minutes);

    swprintf(init_departure, DBL, L"%02dh%02dm", current_stop->departure_time.hours, current_stop->departure_time.minutes);

    int page_state = 0;
    int throw_popup = 0;

    PageArgs args = {
        .style = style,
        .persistence = &persistence,
        .state = 0,
        .throw_popup = 0
    };

    wchar_t error[256] = L"\0";

    int running = 1;
    HandleResult handle_results = {
        .state = state_stops,
        .first_value = line_node
    };

    while(running)
    {
        // Passamos os valores ATUAIS para servirem de padrão caso a persistência esteja vazia
        PageResult result = init_editstop_page(args, error, current_stop->nome, init_arrival, init_departure);

        args.throw_popup = 0; 
        error[0] = L'\0';

        switch (result.action)
        {
        case page_action_back:
            handle_results.state = state_stops;
            running = 0;
            break;

        case page_action_text:
            Hours new_arrival;
            Hours new_departure;

            if (is_emptyw(result.first_text) || 
                is_emptyw(result.second_text) || 
                is_emptyw(result.third_text) ||
                !string_to_time(result.second_text, &new_arrival) ||
                !string_to_time(result.third_text, &new_departure)
            )
            {
                args.throw_popup = 1;
                wcscpy(error, L"Dados inválidos");
                break;
            }

            wcscpy(current_stop->nome, result.first_text);
            current_stop->arrival_time = new_arrival;
            current_stop->departure_time = new_departure;

            handle_results.state = state_stops;
            running = 0;
            break;

        default:
            break;
        }
    }

    if(persistence) free(persistence);

    return handle_results;
}