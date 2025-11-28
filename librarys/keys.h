/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef KEYS_H
#define KEYS_H

#include <wchar.h>

/// @brief Enumerador para teclas
typedef enum Key
{
    unknown,
    esc,
    resize,
    backspace,
    enter,
    up,
    down,
    left,
    right,
    number,
    common
} Key;

/// @brief Função para pegar uma tecla de entrada e retornar uma tecla do enumerador de teclas
/// @param key A tecla
/// @param status O estado dela
/// @return Retorna uma tecla do enumerador
/// @note Se a tecla não estiver mapeada então ele retornara uma tecla desconhecida unknown
Key get_key(wint_t key, int status);

#endif //KEYS_H