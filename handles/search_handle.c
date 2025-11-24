#include <search_handle.h>
#include <search_page.h>
#include <stdlib.h>

States init_search_handle(Style const *style)
{
    //Ponteiro para memória persistente da página
    void *persistence = NULL;

    //Estado da página
    int state = 0;

    //Vetores que serão usados
    wchar_t infomations[] = L"Nada ainda1";
    wchar_t *elements[] = {L"Nada ainda4", L"Nada ainda5"};

    //Tamanho do vetor
    int elements_length = sizeof(elements) / sizeof(elements[0]);

    //Loop principal
    int running = 1;
    States state;

    while(running)
    {
        PageResult result = init_search_page(style, &persistence, state, infomations, elements, elements_length);

        //Manipulação do resultado da página

        switch (result.action)
        {
        case page_action_back:
            running = 0;
            state = state_main;
            break;

        default:
            break;
        }
    }

    if (!persistence)
    {
        free(persistence);
        persistence = NULL;
    }

    return state;
}