#include <search_handle.h>
#include <search_page.h>
#include <stdlib.h>

#define DBL 256
#define TIME_DBL 7

int is_valid_time(wchar_t time[])
{
    return 0;
}

int free_elements(wchar_t *elements[], int elements_length)
{
    for (int index = 0; index < elements_length; index++)
        free(elements[index]);
}

HandleResult init_search_handle(Style const *style)
{
    void *persistence[] = {NULL, NULL};
    int current_persistence = 0;

    //Estado da página
    int page_state = 0;
    int throw_popup = 0;

    //Vetores que serão usados
    wchar_t first_stop[DBL] = {0};
    wchar_t second_stop[DBL] = {0};
    wchar_t first_time[TIME_DBL] = {0};
    wchar_t second_time[TIME_DBL] = {0};

    wchar_t *infomations = NULL;

    wchar_t **elements = NULL;

    //Tamanho do vetor
    int elements_length = 0;

    //Loop principal
    int running = 1;
    HandleResult handle_result = 
    {
        .state = state_main,
        .first_value = NULL
    };

    while(running)
    {
        PageResult result = init_search_page(style, &persistence[current_persistence], page_state, throw_popup, infomations, elements, elements_length);
        throw_popup = 0;

        //Manipulação do resultado da página
        switch (result.action)
        {
        //Caso seja uma ação de voltar
        case page_action_back:
            switch (page_state)
            {
            case 0:
                //Apaga a lista de elementos
                // free_elements(elements, elements_length);

                //E finaliza
                running = 0;
                handle_result.state = state_main;
                break;

            case 1:
                //Atualiza para o primeiro estado
                page_state--;
                current_persistence--;

                //Apaga a lista anterior
                // free_elements(elements, elements_length);

                //Busca a parada anterior com o nome exato
                //search_str(&elements, &elements_length, first_stop);
                break;

            case 2:
                page_state--;
                free(infomations);
                infomations = NULL;
                break;
            
            default:
                break;
            }
            break;

        //Caso seja um texto
        case page_action_text:
            switch (page_state)
            {
            case 0:
            case 1:
                //Apago a lista antiga
                // free_elements(elements, elements_length);

                //Busco uma nova com o novo texto do resultado
                //search_str(&elements, &elements_length, result.text);
                break;
            
            default:
                break;
            }
            break;

        //Caso seja um texto e um índice
        case page_action_text_and_selected:
            switch (page_state)
            {
            case 0:
                //Válida o horário
                if (!is_valid_time(result.first_text))
                {
                    throw_popup = 1;
                    break;
                }

                //Atualiza os estados para o segundo nível
                page_state++;
                current_persistence++;

                //Salva a primeira parada e horário
                wcscpy(first_stop, elements[result.selected_index]);
                wcscpy(first_time, result.first_text);

                //Libera a lista antiga
                // free_elements(elements, elements_length);
                elements = NULL;

                //Apaga um possível persistance anteriormente usado
                free(persistence[current_persistence]);
                persistence[current_persistence] = NULL;
                break;

            case 1:
                //Atualiza o estado para o terceiro nível
                page_state++;

                //Sem break porque aqui quando ele transiciona entre o estado 1 e 2,
                //ele faz a mesma coisas com a diferença da mudança do estado
            case 2:
                //Válida o horário
                if (!is_valid_time(result.first_text))
                {
                    throw_popup = 1;
                    break;
                }

                //Salva a segunda parada
                wcscpy(second_stop, elements[result.selected_index]);
                wcscpy(second_time, result.first_text);

                //Faz a busca
                //infomation = search_route(fist_stop, second_stop, first_time, second_time);
                break;

            default:
                break;
            }

            break;

        default:
            break;
        }
    }

    for(int i = 0; i < 3; i++)
        free(persistence[i]);

    return handle_result;
}