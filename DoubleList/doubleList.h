#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

/**
 * @defgroup DoubleLinkedList
 * @brief Lista Duplamente Encadeada Circular.
 * @warning Para utilizar, deve declarar os dados de DataType e a funcao comp,
 * que sera usada para comparar os DataTypes.
 *
 */

/**
 * @addtgroup DoubleLinkedList
 *
 * @{
 */

/**
 * @strct DataType
 * @brief Struct que sera utilizado para armazenar as informacoes.
 * @warning Devera ser definido pelo usuario.
 */
typedef struct DoubleLinkedListData DataType;

typedef struct DoubleLinkedListNode {
    DataType *info;
    struct DoubleLinkedListNode *next;
    struct DoubleLinkedListNode *prev;
} DoubleLinkedListNode;

typedef struct DoubleLinkedList{
    DoubleLinkedListNode *head;
    int size;
} DoubleLinkedList;


/**
 * @brief Cria a lista.
 * @param Recebe o ponteiro de uma DoubleLinkedList.
 */
void create(DoubleLinkedList *list);

/**
 * @brief Insere 1 Node no fim da lista.
 * @param Recebe como parametro 1 ponteiro DoubleLinkedList e o 1 ponteiro
 * DataType para ser inserido na lista.
 * @return retorna 1 se conseguir inserir ou 0 caso nao consiga.
 */
int append(DoubleLinkedList *dl, DataType *data);

/**
 * @brief Remove Node.
 * @param Recebe como parameteo o no para ser removido
 */
void removeNode(DoubleLinkedListNode *n);

/**
 * @brief Avisa se a lista esta vazia.
 * @param Recebe como parametro 1 ponteiro de uma DoubleLinkedList
 * @return Retorna um se esta vazia e 0 se nao.
 */
int isEmpty(DoubleLinkedList *list);

/** @} */

#endif
