/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef SIMPLE_LINKED_LIST_H
#define SIMPLE_LINKED_LIST_H
#include <stdlib.h>
#include <stdio.h>

typedef struct SimpleLinkedListNode
{
    void *info;
    struct SimpleLinkedListNode *next;
} SimpleLinkedListNode;

typedef struct SimpleLinkedList
{
    struct SimpleLinkedListNode *head;
} SimpleLinkedList;

/// @brief Retorna o número de elementos em uma lista.
/// @param L Ponteiro para a lista.
/// @return O número de elementos na lista.
int get_size_sll(SimpleLinkedList *L);

/// @brief Inicializa uma lista, definindo o número de elementos como 0.
/// @param L Ponteiro para a lista.
int define_sll(SimpleLinkedList* L);

/// @brief Inicializa uma lista de frequência, definindo o número de elementos como 0.
/// @param L Ponteiro para a lista de frequência.
//void defineCountList(freqList* L);

/// @brief Verifica se a lista de elementos está vazia.
/// @param L Ponteiro para a lista de elementos.
/// @return 1 se a lista estiver vazia, 0 caso contrário.
int empty_sll(SimpleLinkedList *L);

/// @brief Insere um elemento no início de uma lista.
/// @param L Ponteiro para a lista.
/// @param V Elemento a ser inserido.
/// @return 1 se a inserção for bem-sucedida, 0 se a lista estiver cheia.
int init_insert_sll(SimpleLinkedList *L, void *V);

/// @brief Insere um elemento no final de uma lista.
/// @param L Ponteiro para a lista.
/// @param V Elemento a ser inserido.
/// @return 1 se a inserção for bem-sucedida, 0 se a lista estiver cheia.
int end_insert_sll(SimpleLinkedList *L, void *V);

/// @brief Remove o primeiro elemento de uma lista.
/// @param L Ponteiro para a lista.
/// @return 1 se a remoção for bem-sucedida, 0 se a lista estiver vazia.
int init_remove_sll(SimpleLinkedList *L);

/// @brief Remove o último elemento de uma lista.
/// @param L Ponteiro para a lista.
/// @return 1 se a remoção for bem-sucedida, 0 se a lista estiver vazia.
int end_remove_sll(SimpleLinkedList *L);

/// @brief Esvazia uma lista, definindo o número de elementos como 0.
/// @param L Ponteiro para a lista.
void destroy_sll(SimpleLinkedList *L);


#endif //SIMPLE_LINKED_LIST_H