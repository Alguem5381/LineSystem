#include <search_handle.h>
#include <search_page.h>
#include <stdlib.h>
#include <hours.h>
#include <letter.h>

#define DBL 256
#define TIME_DBL 7

HandleResult init_search_handle(Style const *style, Object *data)
{
    // Ponteiro para a memoria persistente
    void *persistence[3] = {NULL, NULL, NULL};
    int current_persistence = 0;

    int page_state = 0;
    int throw_popup = 0;

    wchar_t first_stop[DBL] = {0};
    
    // Texto informativo para a tela de resultados
    wchar_t infomations[DBL] = {0};
    
    // Arrays dinâmicos
    wchar_t **strings = NULL;
    int elements_length = 0;
    strings = create_unique_names_list(data, &elements_length, NULL);

    int running = 1;
    HandleResult handle_result = { .state = state_main };

    while(running)
    {
        PageResult result = init_search_page(
            style, 
            &persistence[current_persistence], 
            page_state, 
            throw_popup, 
            infomations, 
            strings, 
            elements_length
        );
        
        throw_popup = 0;

        switch (result.action)
        {
        case page_action_back:
            if (page_state == 0) {
                running = 0;
                handle_result.state = state_main;
            } 
            else if (page_state == 1) {
                // Voltar do destino para a origem

                // Limpa persistência do estado atual
                free(persistence[current_persistence]);
                persistence[current_persistence] = NULL;
                
                current_persistence--;
                page_state--;

                // Recarrega a lista completa sem filtro para a seleção da origem
                free(strings);
                strings = NULL;
                strings = create_unique_names_list(data, &elements_length, NULL);
            }
            else if (page_state == 2) {
                // Voltar do resultado para destino
                free(persistence[current_persistence]);
                persistence[current_persistence] = NULL;

                current_persistence--;
                page_state--;
                
                // Limpa informações de resultado
                infomations[0] = L'\0';

                free(strings);
                strings = NULL;
                strings = create_unique_names_list(data, &elements_length, NULL);
            }
            break;

        case page_action_text:
            // O usuário digitou algo na busca
            if (page_state == 0 || page_state == 1)
            {
                free(strings);
                strings = NULL;
                strings = create_unique_names_list(data, &elements_length, result.first_text);
            }
            break;

        case page_action_text_and_selected:
            if (page_state == 0) // Origem Selecionada
            {
                // Verifica índice válido
                if (result.selected_index < 0 || result.selected_index >= elements_length) break;

                // Salva o nome da parada de origem
                wcscpy(first_stop, strings[result.selected_index]);

                // Avança estado
                page_state++;
                current_persistence++;

                free(strings);
                strings = NULL;
                strings = create_unique_names_list(data, &elements_length, result.first_text);
            }
            else if (page_state == 1) // Destino Selecionado
            {
                // Valida horário
                if (!is_valid_format(result.first_text))
                {
                    throw_popup = 1;
                    break;
                }

                if (result.selected_index < 0 || result.selected_index >= elements_length) break;

                // Prepara busca de rota
                Hours arrival;
                string_to_time(result.first_text, &arrival);
                
                RouteResult routeResult = find_best_route(data, first_stop, strings[result.selected_index], arrival);

                // Formata Resultado
                if (routeResult.found) {
                    swprintf(infomations, DBL, 
                        L"Linha: %ls\nEmpresa: %ls\n\nOrigem: %ls\nDestino: %ls\n\nSaída: %02dh%02dm\nChegada: %02dh%02dm", 
                        routeResult.line_name, 
                        routeResult.enterprise, 
                        routeResult.origin_name,
                        routeResult.dest_name,
                        routeResult.departure.hours, routeResult.departure.minutes,
                        routeResult.arrival.hours, routeResult.arrival.minutes
                    );
                } else {
                    swprintf(infomations, DBL, L"Nenhuma rota encontrada para este horário/trajeto.");
                }

                // Avança para tela de resultado
                page_state++;
                current_persistence++;
            }
            break;

        default:
            break;
        }
    }

    // Limpeza Final
    for(int i = 0; i < 3; i++) {
        if(persistence[i]) free(persistence[i]);
    }

    free(strings);
    strings = NULL;

    return handle_result;
}