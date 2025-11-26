#include <template_handle.h>
#include <template_page.h>

States init_template_handle(Style const *style)
{
    //Loop principal

    int running = 1;
    States state;

    while(running)
    {
        PageResult result = init_template_page(style);

        //Manipulação do resultado da página

        switch (result.action)
        {
        case page_action_back:
            running = 0;
            state = state_exit;
            break;

        default:
            break;
        }
    }

    return state;
}