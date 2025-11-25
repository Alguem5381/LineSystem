#include <main_handle.h>
#include <main_page.h>
#include <stdlib.h>

States init_main_handle(Style const *style)
{
    PageArgs args =
    {
        .style = style,
        .persistence = NULL,
        .state = 0,
        .throw_popup = 0
    };

    //Variaveis do loop
    int running = 1;
    States state;

    while(running)
    {
        PageResult result = init_main_page(args);

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

    return state;
}