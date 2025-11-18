#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

/**
 * @defgroup DoubleList
 * @brief Lista Duplamente Encadeada Circular.
 * @warning Para utilizar, deve declarar os dados de DataType e a funcao comp,
 * que sera usada para comparar os DataTypes.
 *
 */

/**
 * @addtgroup DoubleList
 *
 * @{
 */

/**
 * @strct DataType
 * @brief Struct que sera utilizado para armazenar as informacoes.
 * @warning Devera ser definido pelo usuario.
 */
typedef struct data_type DataType;

typedef struct node {
    DataType *info;
    struct node *next;
    struct node *prev;
} Node;

typedef struct {
    Node *head;
    int size;
} DoubleList;


/**
 * @brief Cria a lista.
 * @param Recebe o ponteiro de uma DoubleList.
 */
void create(DoubleList *list);

/**
 * @brief Insere 1 Node na lista.
 * @param Recebe como parametro 1 ponteiro DoubleList e o 1 ponteiro
 * DataType para ser inserido na lista.
 * @return retorna 1 se conseguir inserir ou 0 caso nao consiga.
 */
int add(DoubleList *list, DataType *data);

/**
 * @brief Remove 1 elemento da lista.
 * @param Recebe como parametro 1 ponteiro DoubleLIst e o 1 ponteiro
 * DataTyp que deseja remover da lista.
 * @return retorna 1 se conseguir remover, 0 casa nao achou o DataType ou -1 se
 * a lista estiver vazia.
 */
int removeData(DoubleList *list, DataType *data);

/**
 * @brief Remove Node.
 * @param Recebe como parameteo o no para ser removido
 */
void removeNode(Node *n);

/**
 * @brief Procurar elemento na lista.
 * @param Recebe como parametro 1 ponteiro DoubleLIst e o 1 ponteiro DataTyp que
 * deseja procurar na lista.
 * @return Retorna um ponteiro constante do elemento achado, evitando que o
 * conteudo seja alterado, ou NULL, caso o elemento nao seja encontrado.
 */
DataType const *search(DoubleList *list, DataType *data);

/**
 * @brief Avisa se a lista esta vazia.
 * @param Recebe como parametro 1 ponteiro de uma DoubleList
 * @return Retorna um se esta vazia e 0 se nao.
 */
int isEmpty(DoubleList *list);

/**
 * @brief Compara elementos na lista, devera ser difinida pelo usuario.
 * @param Recebe como parametro 2 ponteiros DataType
 * @warning Essa funcao sera usada nas operacoes da lista!
 */
int comp(DataType *data1, DataType *data2);

/**
 * @brief Exibe a info dos elementos da lista.
 * @param Recebe como parametro 1 ponteiro de uma DoubleList
 * @warning Essa funcao exibira os dados usando a funcao showData(), que deve
 * ser definida pelo usuario!
 */
void show(DoubleList *list);

/**
 * @brief Exibe os dados do DataType, devera ser difinida pelo usuario.
 * @param recebe como parametro 1 ponteiro DataType
 * @warning Essa funcao sera usada nas operacoes da lista!
 */
void showData(DataType *data);

/** @} */

#endif
