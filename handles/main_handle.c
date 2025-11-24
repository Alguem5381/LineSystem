#include <main_handle.h>
#include <main_page.h>
#include <stdlib.h>

States init_main_handle(Style const *style)
{
    //Loop principal
    void *persistence = NULL;

    //Variaveis do loop
    int running = 1;
    States state;

    while(running)
    {
        PageResult result = init_main_page(style, &persistence);

        //Manipulação do resultado da página

        switch (result.action)
        {
        case page_action_back:
            state = state_exit;
            running = 0;
            break;

        case page_action_select:
            switch (result.selected_index)
            {
            case 0: 
                state = state_search;
                break;

            case 1: 
                state = state_login; 
                break;

            default:
                break;
            }

            running = 0;
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