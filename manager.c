/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <states.h>
#include <style.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <fileMenager.h>
#include <object.h>

//Handles
#include <main_handle.h>
#include <search_handle.h>
#include <login_handle.h>
#include <line_handle.h>
#include <stop_handle.h>
#include <newline_handle.h>
#include <newstop_handle.h>
#include <editstop_handle.h>

/// @brief Função para iniciar o ncurses
/// @return 1 se sucesso 0 se falha
int init_ncurses()
{
    // Configura a localidade
    setlocale(LC_ALL, "");

    //Espande o terminal para 40x160
    printf("\x1b[8;40;160t");
    fflush(stdout);

    initscr();              //Inicia a tela
    cbreak();               //Remove a espera do enter
    curs_set(0);            //Deixa o cursor invisível
    noecho();               //Não permite o terminal desenhar a tecla digitada na tela
    keypad(stdscr, true);   //Ativa a detecção de teclas especiais
    start_color();          //Inicia o sistema de cores do ncurses

    if(!has_colors())       //Verifica se há cores
    {
        endwin();
        printf("Não suporta cores\n");
        return 0;
    }

    refresh();

    return 1;
}


/// @brief Finaliza o ncurses e retorna a visibilidade do cursor
/// @return 1 se sucesso e 0 se falha
int end_ncurses()
{
    curs_set(1);
    endwin();       //Finaliza a tela
    return 1;
}

/// @brief Inicializa as cores
/// @param style O estilo que receberá os valores de cada par
/// @return 1 se sucesso e 0 se falha
int init_style(Style *style)
{
    if (!style) return 0;

    //init_pair() inicializa o par de cor
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_CYAN, COLOR_CYAN);
    init_pair(4, COLOR_WHITE, COLOR_CYAN);
    init_pair(5, COLOR_WHITE, COLOR_CYAN);

    style->default_pair_color = 1;
    style->default_border_color = 2;
    style->on_focus_border_color = 3;
    style->on_focus_pair_color = 4;
    style->divider_color = 5;

    return 1;
}

int main()
{
    //Structs
    Style style;
    Data data;

    // Inicializa o objeto com NULL antes de carregar
    defineObject(&data);

    //Funções de configuração do programa e structs 
    int result = 1;

    // Carrega os dados da memória
    loadData(&data);

    if (!init_ncurses())
    {
        printf("Falha ao iniciar o ncurses\n");
        result = 0;
    }

    if (!init_style(&style))
    {
        printf("Falha ao iniciar o estilo\n");
        end_ncurses();
        result = 0;
    }

    if (!result) 
    {
        deleteObject(&data);
        return 1;
    }

    //Loop principal
    int running = 1;

    HandleResult handle_result = {
        .state = state_main,
        .first_value = NULL,
        .second_value = NULL,
        .third_value = NULL
    };

    while(running)
    {
        // Tratamento dos estados
        switch (handle_result.state)
        {
        case state_main:
            handle_result = init_main_handle(&style);
            break;

        case state_search:
            handle_result = init_search_handle(&style, &data);
            break;

        case state_login:
            handle_result = init_login_handle(&style);
            break;

        case state_line:
            handle_result = init_line_handle(&style, &data);
            break;

        case state_stops:
            handle_result = init_stop_handle(&style, handle_result.first_value);
            break;

        case state_new_line:
            handle_result = init_newline_handle(&style, &data);
            break;

        case state_new_stop:
            handle_result = init_newstop_handle(&style, handle_result.first_value, handle_result.third_value);
            break;

        case state_edit_stop:
            handle_result = init_editstop_handle(&style, handle_result.second_value, handle_result.first_value);
            break;

        case state_exit:
            running = 0;
            break;
        
        default:
            running = 0; // Segurança contra estados inválidos
            break;
        }
    }

    // Não pode liberar os dados do handle_result porque eles
    // fazem parte dos dados carregados da memória

    end_ncurses();

    if (!saveObject(&data))
        printf("Falha ao salvar dados.\n");

    deleteObject(&data);

    return 0;
}