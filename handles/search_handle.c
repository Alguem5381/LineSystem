#include <search_handle.h>
#include <search_page.h>

States init_search_handle(Style const *style)
{
    //Loop principal

    int running = 1;
    States state;

    wchar_t infomations[] = L"Nada ainda1";
    wchar_t search_bar_text[] = L"Pesquise uma parada";
    wchar_t time_bar_text[] = L"Hora de chegada/saida. (Ex: 19h30m)";
    wchar_t *elements[] = {L"Nada ainda4", L"Nada ainda5"};

    int elements_length = sizeof(elements) / sizeof(elements[0]);

    while(running)
    {
        PageResult result = init_search_page(style, search_bar_text, time_bar_text, infomations, elements, elements_length);

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

    return state;
}