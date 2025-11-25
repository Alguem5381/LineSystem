#ifndef SEARCH_H
#define SEARCH_H

#include <fileMenager.h>
#include <doubleList.h>
#include <simpleLinkedList.h>
#include <wchar.h>
#include <hours.h>

/// @brief Função para buscar e montar um vetor com nomes de paradas parecidas com uma string específica.
/// @param information Os dados dados para a busca
/// @param array O ponteiro para o vetor de string(nome das paradas)
/// @param array_length O tamanho do vetor
/// @param string A string específica
/// @return Retorna 1 se for possível montar o vetor e 0 em caso de falha
int search_to_array(Object *information, wchar_t ***array, int *array_length, wchar_t *string);

/// @brief Função para buscar uma rota entre duas paradas com horários alvos de saida e chegadas e retornar uma string com todas as informações dessa rota.
/// @param information Os dados para a busca
/// @param first_stop O nome da primeira parada
/// @param second_stop O nome da segunda parada
/// @param departure_time O horário de saida esperado
/// @param arrival_time O horário de chegada esperado
/// @return Retorna uma string contendo as informações caso encontrar ou uma string com o aviso de falha na busca(Ex: "Não foi possível encontrar").
wchar_t *search_route(Object *information, wchar_t *first_stop, wchar_t *second_stop, Hours departure_time, Hours arrival_time);

#endif //SEARCH_H
