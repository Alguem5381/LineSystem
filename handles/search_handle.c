#include <search_handle.h>
#include <search_page.h>
#include <stdlib.h>

States init_search_handle(Style const *style)
{
    void *persistence[] = {NULL, NULL, NULL};

    //Estado da página
    int page_state = 0;

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
        PageResult result = init_search_page(style, &persistence[page_state], page_state, infomations, elements, elements_length);

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

    for(int i = 0; i < 3; i++)
        free(persistence[i]);

    return state;
}