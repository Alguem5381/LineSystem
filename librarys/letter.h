#ifndef LETTER_H
#define LETTER_H

#include <wchar.h>

/// @brief Verifica se o caracter é uma letra
/// @param character O caracter
/// @return Retorna 1 se caso sim e 0 caso contrário
int is_letter(char const character);

/// @brief Verifica se o caracter largo é uma letra
/// @param character O caracter
/// @return Retorna 1 se caso sim e 0 caso contrário
int is_wletter(wchar_t const character);

/// @brief Verifica se o caracter é um numero 
/// @param character O caracter
/// @return Retorna 1 se caso sim e 0 caso contrário
int is_number(char const character);

/// @brief Verifica se o caracter largo é um numero 
/// @param character O caracter
/// @return Retorna 1 se caso sim e 0 caso contrário
int is_wnumber(wchar_t const character);

/// @brief Remove o ultimo caracter de uma string larga
/// @param string A string
/// @return Retorna 1 se sucesso e 0 caso contrário
int remove_lastw(wchar_t string[]);

/// @brief Adiciona um caracter na ultima posição de uma string larga
/// @param string A string
/// @param length O tamanho da string
/// @param character O caracter
/// @return Retorna 1 se sucesso e 0 caso contrário
int add_lastw(wchar_t string[], int length, wchar_t character);

/// @brief Verifica se a string larga está vazia
/// @param string A string
/// @return Retorna 1 se sim e 0 se não
int is_emptyw(wchar_t *string);

/// @brief Uma função wrapper para wcsstr para melhor segurança
/// @param needle A sub-string
/// @param haystack A string completa
/// @return Retorna o resultado de wcsstr
int contains_wstr(wchar_t *needle, wchar_t *haystack);

/// @brief Desaloca uma array de string
/// @param string_array O vetor de string
/// @param length O tamanho do vetor
void free_string_array(wchar_t **string_array, int length);

#endif //LETTER_H