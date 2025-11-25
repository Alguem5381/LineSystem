#ifndef SIMPLE_LINKED_LIST
#define SIMPLE_LINKED_LIST
#include <stdlib.h>
#include <stdio.h>

typedef struct SLL_Data SLLData;


typedef struct SLL_Node
{
    void *info;
    struct SLL_Node *next;
} SLL_Node;

typedef struct
{
    struct SLL_Node *head;
} linkedList;

/// @brief Retorna o número de elementos em uma lista.
/// @param L Ponteiro para a lista.
/// @return O número de elementos na lista.
int size(linkedList *L);

/// @brief Inicializa uma lista, definindo o número de elementos como 0.
/// @param L Ponteiro para a lista.
int define(linkedList* L);

/// @brief Inicializa uma lista de frequência, definindo o número de elementos como 0.
/// @param L Ponteiro para a lista de frequência.
//void defineCountList(freqList* L);

/// @brief Verifica se a lista está cheia.
/// @param L Ponteiro para a lista.
/// @return 1 se a lista estiver cheia, 0 caso contrário.
int full(linkedList *L);

/// @brief Verifica se a lista de elementos está vazia.
/// @param L Ponteiro para a lista de elementos.
/// @return 1 se a lista estiver vazia, 0 caso contrário.
int emptyList(linkedList *L);


/// @brief Verifica se uma lista está ordenada em ordem crescente.
/// @param L Ponteiro para a lista.
/// @return 1 se a lista estiver ordenada, 0 caso contrário.
int isOrder(linkedList *L);

/// @brief Insere um elemento em uma lista ordenada, mantendo a ordem.
/// @param L Ponteiro para a lista.
/// @param V Elemento a ser inserido.
/// @return 1 se a inserção for bem-sucedida, 0 se a lista estiver cheia.
int orderInsert(linkedList *L, void *V);


/// @brief Insere um elemento no início de uma lista.
/// @param L Ponteiro para a lista.
/// @param V Elemento a ser inserido.
/// @return 1 se a inserção for bem-sucedida, 0 se a lista estiver cheia.
int initInsert(linkedList *L, void *V);

/// @brief Insere um elemento no final de uma lista.
/// @param L Ponteiro para a lista.
/// @param V Elemento a ser inserido.
/// @return 1 se a inserção for bem-sucedida, 0 se a lista estiver cheia.
int endInsert(linkedList *L, void *V);

/// @brief Remove o primeiro elemento de uma lista.
/// @param L Ponteiro para a lista.
/// @return 1 se a remoção for bem-sucedida, 0 se a lista estiver vazia.
int initRemove(linkedList *L);

/// @brief Remove o último elemento de uma lista.
/// @param L Ponteiro para a lista.
/// @return 1 se a remoção for bem-sucedida, 0 se a lista estiver vazia.
int endRemove(linkedList *L);

/// @brief Esvazia uma lista, definindo o número de elementos como 0.
/// @param L Ponteiro para a lista.
void destroy(linkedList *L);


#endif