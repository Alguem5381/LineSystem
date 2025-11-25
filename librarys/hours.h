#ifndef HOURS_H
#define HOURS_H

#include <wchar.h>

/// @defgroup Hours
/// @brief Tipo e funções para manipular horários
/// @{

/// @brief Tipo para horários
typedef struct Hours
{
    int hours;
    int minutes;
} Hours;

/// @brief Função para verificar se o horário dentro da string é válido. (Ex: 19h30m)
/// @param string A string
/// @return Retorna 1 se for válido ou 0 caso contrário
int is_valid_time(const wchar_t *string);

/// @brief Função para escrever um horário em uma string.
/// @param string A string
/// @param string_length O tamanho da string
/// @param time O horário
/// @return Retorna 1 se sucesso ou 0 em caso de falha
int time_to_string(wchar_t *string, const int string_length, const Hours time);

/// @brief Função para colocar um horário de uma string em uma variável de horário.
/// @param string A string
/// @param time O ponteiro para o horário
/// @return Retorna 1 se sucesso ou 0 em caso de falha
int string_to_time(const wchar_t *string, Hours *time);

/// @brief Função para conseguir o horário seguinte mais próximo de um horário alvo em um vetor de horários
/// @param array O vetor de horários
/// @param array_lenght O tamanho do vetor
/// @param target O horário alvo
/// @return Retorna a posição no vetor se sucesso ou -1 em caso de falha
int get_next_nearest_time(Hours *array, int array_lenght, Hours target);

/// }@ Fim do grupo Hours

#endif //HOURS_H