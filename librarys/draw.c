#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <draw.h>

typedef struct Coordinates
{
    int initial_y;         // Coordenada y onde o desenho deve iniciar. Tal valor é a subtração da coordenada central pela metade da altura
    int initial_x;         // Coordenada x onde o desenho deve iniciar. Tal valor é a subtração da coordenada central pela metade da largura
    int final_y;           // Coordenada y onde o desenho deve terminar. Tal valor é a adição da coordenada central pela subtração entre a metade da altura e 1
    int final_x;           // Coordenada x onde o desenho deve terminar. Tal valor é a adição da coordenada central pela subtração entre a metade da largura e 1
    int horizontal_middle; // Coordenada do meio horizontal do desenho
    int vertical_middle;   // Coordenada do meio vertical do desenho
    int width;             // Largura do desenho
    int height;            // Altura do desenho
} Coordinates;

// Funções auxiliares

// Define coordenadas
int define_coordinates(Coordinates *coordinates, DrawContext const *context, int min_height, int min_width)
{
    // Tamanho do espaço disponível para desenhar a caixa
    int avaliable_width = context->endx - context->startx;
    int avalible_height = context->endy - context->starty;

    // Tamanho esperado para a caixa
    coordinates->height = (int)((avalible_height * context->height) / 100);
    coordinates->width = (int)((avaliable_width * context->width) / 100);

    // Se não tiver a altura e largura mínima então não desenha
    if (coordinates->height < min_height || coordinates->width < min_width)
        return 0;

    // Para saber o meio da área de desenho
    coordinates->horizontal_middle = (context->endx + context->startx) / 2;
    coordinates->vertical_middle = (context->endy + context->starty) / 2;

    // Valores x e y para determinar o inicio e fim da caixa relativa ao centro
    coordinates->initial_y = coordinates->vertical_middle - coordinates->height / 2;
    coordinates->initial_x = coordinates->horizontal_middle - coordinates->width / 2;
    coordinates->final_y = coordinates->vertical_middle + coordinates->height / 2 - 1;
    coordinates->final_x = coordinates->horizontal_middle + coordinates->width / 2 - 1;

    return 1;
}

// Desenha uma caixa
int draw_box(int startx, int starty, int endx, int endy)
{
    for (int y = starty; y <= endy; y++)
        mvhline(y, startx, ' ', endx - startx + 1);
}

// Desenha bordas no formato de caixa
int draw_frame_box(int startx, int starty, int endx, int endy)
{
    for (int x = startx; x <= endx; x++)
    {
        mvaddwstr(starty, x, L"▒");
        mvaddwstr(endy, x, L"▒");
    }

    wchar_t temp[3];

    // As bordas laterais possuem 2 pixels de espessura, logo, para
    // evita desenhar em um x negativo eu coloque esse if (Ex: startx == 0)
    if (!startx)
        wcscpy(temp, L"▒");
    else
    {
        wcscpy(temp, L"▒▒");
        startx--;
    }

    // Antes a borda direita poderia ser desenha fora da tela do terminal em -1
    for (int y = starty; y <= endy; y++)
    {
        mvaddwstr(y, startx, temp);
        mvaddwstr(y, endx, L"▒▒");
    }
}

int draw_text_box(wchar_t const *text, DrawContext const *context)
{
    Coordinates coordinates;

    if (!define_coordinates(&coordinates, context, 3, 9) || !text || !context)
        return 0;

    attron(COLOR_PAIR(context->default_pair_color));
    draw_box(coordinates.initial_x, coordinates.vertical_middle - 1, coordinates.final_x, coordinates.vertical_middle + 1);

    mvaddnwstr(coordinates.vertical_middle, coordinates.initial_x + 1, text, coordinates.width - 2);
    attroff(COLOR_PAIR(context->default_pair_color));

    // Nesse caso, o elemento em foco é usado diferente. Caso seja diferente de 0, a caixa fica com a borda em foco
    if (context->element_in_focus)
    {
        attron(COLOR_PAIR(context->on_focus_border_color));
        draw_frame_box(coordinates.initial_x, coordinates.vertical_middle - 1, coordinates.final_x, coordinates.vertical_middle + 1);
        attroff(COLOR_PAIR(context->on_focus_border_color));
    }
    else
    {
        attron(COLOR_PAIR(context->default_border_color));
        draw_frame_box(coordinates.initial_x, coordinates.vertical_middle - 1, coordinates.final_x, coordinates.vertical_middle + 1);
        attroff(COLOR_PAIR(context->default_border_color));
    }

    return 1;
}

int draw_base_page(wchar_t const *title, DrawContext const *context)
{
    if (context->endx < 10 || context->endy < 8 || !title || !context || wcslen(title) + 4 > context->endx)
        return 0;

    attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
    // Desenha a página
    draw_box(0, 0, context->endx, context->endy);
    // Escreve o texto
    mvaddwstr(1, 2, title);
    attroff(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão

    attron(COLOR_PAIR(context->divider_color)); // Ativa a cor de separador
    for (int i = 0; i < context->endx; i++)     // Desenha a faixa superior da página
        mvaddwstr(3, i, L" ");
    attroff(COLOR_PAIR(context->divider_color)); // Desativa a cor de separador

    return 1;
}

int draw_message_dialog(wchar_t const *message, DrawContext const *context)
{
    Coordinates coordinates;

    // Define as coordenadas para desenho, caso esteja fora do esperado então não desenha
    if (!define_coordinates(&coordinates, context, 10, 8) || !message || !context)
        return 0;

    coordinates.initial_y = coordinates.vertical_middle - 4;
    coordinates.final_y = coordinates.vertical_middle + 4;

    // Tamanho do texto
    int message_length = wcslen(message);

    // A posição x onde o texto inicia
    int message_initial_x = 0;

    // Decide onde o texto inicia no x
    if (message_length <= coordinates.width - 6)
        message_initial_x = coordinates.horizontal_middle - message_length / 2; // Inicia em uma distância relativa ao centro que permite centralizar a mensagem
    else
        message_initial_x = coordinates.horizontal_middle - coordinates.width / 2 + 3; // Inicia a dois pixels da borda esquerda

    attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
    // Desenha a caixa
    draw_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);

    // Verifica se o texto passara da borda
    if (message_initial_x + message_length < coordinates.final_x - 2)
        // Caso não, então ele escreve com o tamanho do texto
        mvaddnwstr(coordinates.vertical_middle - 2, message_initial_x, message, message_length);
    else
        // Caso sim, então ele escreve até dois pixel antes da borda direita
        mvaddnwstr(coordinates.vertical_middle - 2, message_initial_x, message, coordinates.width - 6);
    attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão

    attron(COLOR_PAIR(context->default_border_color)); // Ativa a cor para bordas
    // Desenha a borda da caixa
    draw_frame_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);
    attroff(COLOR_PAIR(context->default_border_color)); // Desativa a cor para bordas

    attron(COLOR_PAIR(context->on_focus_pair_color)); // Ativa a cor de foco
    // Escreve a opção de ok
    mvaddwstr(coordinates.vertical_middle + 1, coordinates.horizontal_middle - 2, L" Ok ");
    attroff(COLOR_PAIR(context->on_focus_pair_color)); // Desativa a cor de foco

    return 1;
}

int draw_yes_or_no_dialog(wchar_t const *message, DrawContext const *context)
{
    Coordinates coordinates;

    // Se a definição de coordenadas falhar ou se os valores de parametros não forem validos então retorna erro
    if (!define_coordinates(&coordinates, context, 10, 8) || !message || !context)
        return 0;

    coordinates.initial_y = coordinates.vertical_middle - 4;
    coordinates.final_y = coordinates.vertical_middle + 4;

    // Tamanho do texto
    int message_length = wcslen(message);

    // A posição x onde o texto inicia
    int message_initial_x = 0;

    // Decide onde o texto inicia no x
    if (message_length <= coordinates.width - 6)
        message_initial_x = coordinates.horizontal_middle - message_length / 2; // Inicia em uma distância relativa ao centro que permite centralizar a mensagem
    else
        message_initial_x = coordinates.horizontal_middle - coordinates.width / 2 + 3; // Inicia a dois pixels da borda esquerda

    attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
    // Desenha a caixa
    draw_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);

    // Verifica se o texto passara da borda
    if (message_initial_x + message_length < coordinates.final_x - 2)
        // Caso não, então ele escreve com o tamanho do texto
        mvaddnwstr(coordinates.vertical_middle - 2, message_initial_x, message, message_length);
    else
        // Caso sim, então ele escreve até dois pixel antes da borda direita
        mvaddnwstr(coordinates.vertical_middle - 2, message_initial_x, message, coordinates.width - 6);
    attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão

    attron(COLOR_PAIR(context->default_border_color)); // Ativa a cor para bordas
    // Desenha a borda da caixa
    draw_frame_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);
    attroff(COLOR_PAIR(context->default_border_color)); // Desativa a cor para bordas

    // Verifica qual o elemento selecionado e desenha a o texto em foco
    if (context->element_in_focus) // 0 para Não e diferente de 0 para Sim
    {
        attron(COLOR_PAIR(context->on_focus_pair_color)); // Ativa a cor de foco
        mvaddwstr(coordinates.vertical_middle + 1, coordinates.horizontal_middle - 4, L" Yes ");
        attroff(COLOR_PAIR(context->on_focus_pair_color)); // Desativa a cor de foco

        attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
        mvaddwstr(coordinates.vertical_middle + 1, coordinates.horizontal_middle + 4, L"No");
        attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão
    }
    else
    {
        attron(COLOR_PAIR(context->on_focus_pair_color)); // Ativa a cor de foco
        mvaddwstr(coordinates.vertical_middle + 1, coordinates.horizontal_middle + 2, L" No ");
        attroff(COLOR_PAIR(context->on_focus_pair_color)); // Desativa a cor de foco

        attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
        mvaddwstr(coordinates.vertical_middle + 1, coordinates.horizontal_middle - 6, L"Yes");
        attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão
    }

    return 1;
}

int draw_footer(wchar_t **keys, wchar_t **options, int array_length, DrawContext const *context)
{
    if (!options || !keys || !array_length || !context || context->endx < 10 || context->endy < 8)
        return 0;

    int text_length = 0;
    for (int i = 0; i < array_length; i++) // Soma o tamanho de todas as opções e teclas
        text_length += wcslen(options[i]) + wcslen(keys[i]);

    // Tamanho vezes 3 visto que no final ficara assim "{key} - {option}" e (tamanho - 1) * 3 por causa da separação entre cada par "{par1}   {par2}"
    // Exemplo com 3 elementos: "Esc - Voltar   Seta esquerda - Move para esquerda   Seta direita - Move para direita"
    text_length += array_length * 3 + (array_length - 1) * 3;

    // Tamanho mínimo para desenho do rodapé com o texto a dois pixels de distância da borda
    if (text_length > context->endx - 4)
        return 0;

    // Onde o texto deve iniciar no eixo x
    int text_initial_x = (context->endx / 2) - (text_length / 2);

    // Desenha a faixa inferior
    attron(COLOR_PAIR(context->divider_color)); // Ativa a cor de divisor
    for (int i = 0; i < context->endx; i++)
        mvaddwstr(context->endy - 4, i, L" ");   // Desenha a faixa inferior
    attroff(COLOR_PAIR(context->divider_color)); // Desativa a cor padrão

    wchar_t buffer[1024]; // Buffer para o texto

    attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
    move(context->endy - 2, text_initial_x);
    for (int i = 0; i < array_length; i++)
    {
        swprintf(buffer, 1024, L"%ls - %ls   ", keys[i], options[i]);
        addwstr(buffer);
    }
    attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão

    return 1;
}

int draw_list(wchar_t **elements, int array_length, DrawContext const *context)
{
    Coordinates coordinates;

    // Se a definição de coordenadas falhar ou se os valores de parametros não forem validos então retorna erro
    if (!define_coordinates(&coordinates, context, 9, 9) || !context)
        return 0;

    attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
    // Desenha a caixa
    draw_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);
    attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão

    wchar_t buffer[1024];

    if (array_length)
    {
        if (coordinates.height / 2 - 1 < context->element_in_focus) // Se o índice do elemento em foco for maior que a metade do tamanho da caixa menos 1
        {
            // Então ele desenha os elementos do centro para o topo da caixa partindo do índice para o início do vetor
            attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
            for (int index = context->element_in_focus, pos_in_y = coordinates.height / 2 + coordinates.initial_y; pos_in_y > coordinates.initial_y && index >= 0; index--, pos_in_y--)
            {
                swprintf(buffer, 1024, L"%-*ls", coordinates.width - 2, elements[index]); // Escreve a string dentro do buffer sem ultrapassar as bordas, por isso coordinates.width - 2
                mvaddwstr(pos_in_y, coordinates.initial_x + 1, buffer);                   // intial_x + 1 por causa da borda
            }

            // Agora ele desenha os elementos do centro para a parte inferior da caixa partindo do indice para o fim do vetor
            for (int index = context->element_in_focus, pos_in_y = coordinates.height / 2 + coordinates.initial_y; pos_in_y < coordinates.final_y && index < array_length; index++, pos_in_y++)
            {
                swprintf(buffer, 1024, L"%-*ls", coordinates.width - 2, elements[index]);
                mvaddwstr(pos_in_y, coordinates.initial_x + 1, buffer);
            }
            attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão

            attron(COLOR_PAIR(context->on_focus_pair_color)); // Ativa a cor de foco
            swprintf(buffer, 1024, L"%-*ls", coordinates.width - 2, elements[context->element_in_focus]);
            mvaddwstr(coordinates.initial_y + coordinates.height / 2, coordinates.initial_x + 1, buffer); // Escreve o texto em foco no centro da caixa
            attroff(COLOR_PAIR(context->on_focus_pair_color));                                            // Desativa a cor de foco
        }
        else
        {
            // Caso contrario, ele desenha todos os elementos do topo da caixa para o fim começando no início do vetor
            attron(COLOR_PAIR(context->default_pair_color));
            for (int index = 0, pos_in_y = 1 + coordinates.initial_y; pos_in_y < coordinates.final_y && index < array_length; index++, pos_in_y++)
            {
                swprintf(buffer, 1024, L"%-*ls", coordinates.width - 2, elements[index]);
                mvaddwstr(pos_in_y, coordinates.initial_x + 1, buffer);
            }
            attroff(COLOR_PAIR(context->default_pair_color));

            // Depois desenha o elemento em foco posicionando ele relativo ao topo da caixa usando o índice
            attron(COLOR_PAIR(context->on_focus_pair_color)); // Ativa a cor de foco
            swprintf(buffer, 1024, L"%-*ls", coordinates.width - 2, elements[context->element_in_focus]);
            mvaddwstr(coordinates.initial_y + context->element_in_focus + 1, coordinates.initial_x + 1, buffer);
            attroff(COLOR_PAIR(context->on_focus_pair_color)); // Desativa a cor de foco
        }
    }

    attron(COLOR_PAIR(context->default_border_color)); // Ativa a cor de borda
    // Desenha a borda da caixa da lista
    draw_frame_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);
    attroff(COLOR_PAIR(context->default_border_color)); // Desativa a cor de borda

    return 1;
}

int draw_label(wchar_t const *text, DrawContext const *context)
{
    Coordinates coordinates;

    // Se a definição de coordenadas falhar ou se os valores de parametros não forem validos então retorna erro
    if (!define_coordinates(&coordinates, context, 6, 6) || !context)
        return 0;

    // Variaveis de posição x e y para manipular a posição do texto
    int pos_y = coordinates.initial_y + 2;
    int pos_x = coordinates.initial_x + 3;

    // Tamanho do texto
    int text_length = 0;
    if (text)
        text_length = wcslen(text);

    // Ponteiro para navegar pelo texto
    wchar_t *dummy = (wchar_t *)text;

    attron(COLOR_PAIR(context->default_pair_color)); // Ativa a cor padrão
    // Desenha a caixa
    draw_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);
    attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão

    if (text)
    {
        attron(COLOR_PAIR(context->default_pair_color));
        // Ele continuara no loop enquanto o ponteiro temporário não passar do endereço da adição entre o endereço inícial do texto e o tamanho dele
        // e em quanto a posição y não chegar a 2 pixel da borda inferior da caixa
        while (dummy < text + text_length && pos_y < coordinates.final_y - 2)
        {
            // Tamanho da primeira palavra no endereço temporário
            int temp = wcscspn(dummy, L" \0");

            // Se na posição atual mais o tamanho da palavra estiver estiver a dois pixels da borda
            if (pos_x + temp > coordinates.final_x - 2)
            {
                pos_y++;                           // Ele desce a posição no y
                pos_x = coordinates.initial_x + 3; // e reseta a posição no x
            }

            // Para evitar desenha uma palavra maior que a largura da caixa
            if (temp < coordinates.width - 4)
                // Na posições indicadas desenha a palavra
                mvaddnwstr(pos_y, pos_x, dummy, temp);

            pos_x += temp; //"Anda" no x para a esquerda
            dummy += temp; // e "Anda" o ponteiro temporario dentro da string principal

            if (*dummy == L' ') // Se nessa posição dentro do texto houver um espaço então ele pula
            {
                dummy++; // Anda o ponteiro
                pos_x++; // e o x
            }
        }
        attroff(COLOR_PAIR(context->default_pair_color)); // Desativa a cor padrão
    }

    attron(COLOR_PAIR(context->default_border_color)); // Ativa a cor de borda
    // Desenha a borda da caixa
    draw_frame_box(coordinates.initial_x, coordinates.initial_y, coordinates.final_x, coordinates.final_y);
    attroff(COLOR_PAIR(context->default_border_color)); // Ativa a cor de borda

    return 1;
}

int split_context(DrawContext *dest, DrawContext const *src, int percent, int half, int direction)
{
    if (!direction) // Se a direção for 0 então divida na horizontal: -|-
    {
        if (!half) // Se a metade for 0 então ele entrega a primeira metade de src para dest
        {
            dest->startx = src->startx;
            dest->endx = src->endx * percent / 100;
            dest->starty = src->starty;
            dest->endy = src->endy;
        }
        else // Caso contrário então entrega a segunda metade para dest
        {
            dest->startx = (src->endx - src->startx) * ((float)percent / 100.0f) + src->startx;
            dest->endx = src->endx;
            dest->starty = src->starty;
            dest->endy = src->endy;
        }
    }
    else // Caso contrário então divide na vertical: ÷
    {
        if (!half) // Se a metade for 0 então ele entrega a primeira metade de src para dest
        {
            dest->startx = src->startx;
            dest->endx = src->endx;
            dest->starty = src->starty;
            dest->endy = src->endy * percent / 100;
        }
        else // Caso contrário então entrega a segunda metade para dest
        {
            dest->startx = src->startx;
            dest->endx = src->endx;
            dest->starty = (src->endy - src->starty) * ((float)percent / 100.0f) + src->starty;
            dest->endy = src->endy;
        }
    }
}