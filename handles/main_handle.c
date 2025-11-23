#include <main_handle.h>
#include <main_page.h>

States init_main_handle(Style const *style)
{
    //Loop principal

    int running = 1;
    States state;

    while(running)
    {
        PageResult result = init_main_page(style);

        //Manipulação do resultado da página

        switch (result.action)
        {
        case page_action_back:
            state = state_exit;
            running = 0;
            break;

        case page_action_select:
            if(result.selected_index == 0)
                state = state_search;
            running = 0;
            break;

        default:
            break;
        }
    }

    return state;
}