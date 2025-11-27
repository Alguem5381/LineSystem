/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
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

/// @brief Função para verificar se o horário está entre 23h59m e 0h0m. (Ex: 19h30m)
/// @param time O horário
/// @return Retorna 1 se for válido ou 0 caso contrário
int is_valid_time(const Hours time);

/// @brief Função para verificar se o horário dentro da string é válido. (Ex: XXhXXm)
/// @param string A string
/// @return Retorna 1 se for válido ou 0 caso contrário
/// @warning Essa função verifica somente o formato. Logo valores incorretos (Ex: 99h99m) não causaram falhas.
int is_valid_format(const wchar_t *string);

/// @brief Função para escrever um horário em uma string.
/// @param buffer A string
/// @param buffer_length O tamanho da string
/// @param time O horário
/// @return Retorna 1 se sucesso ou 0 em caso de falha
/// @warning O tamanho do buffer deve levar em conta o '\0'. Logo é esperado um buffer de tamanho 7
int time_to_string(wchar_t *buffer, const int buffer_length, const Hours time);

/// @brief Função para colocar um horário de uma string em uma variável de horário.
/// @param string A string
/// @param time O ponteiro para o horário
/// @return Retorna 1 se sucesso ou 0 em caso de falha
int string_to_time(const wchar_t *string, Hours *time);

/// @brief Função para tranformar um horas em minutos
/// @param h O horário
/// @return Retorna o hórario em minutos
int to_minutes(Hours h);

/// }@ Fim do grupo Hours

#endif //HOURS_H