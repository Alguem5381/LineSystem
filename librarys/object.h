/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef OBJECT_H
#define OBJECT_H

#include "simpleLinkedList.h"
#include "doubleList.h"
#include <hours.h>

/// @brief Estrutura das linhas
typedef struct SimpleLinkedListData {
    DoubleLinkedList *list;
    wchar_t name[20];
    wchar_t enterprise[20];
} BusLine;

/// @brief Estrutura das paradas
typedef struct DoubleLinkedListData {
    Hours departure_time;
    Hours arrival_time;
    wchar_t name[20];
} BusStop;

/// @brief Estrutura para guardar dados carregados
typedef struct Data {
    SimpleLinkedList *SLL;
} Data;

// Struct para devolver o resultado de buscas
typedef struct RouteResult {
    wchar_t line_name[20];
    wchar_t enterprise[20];
    wchar_t origin_name[20];
    wchar_t dest_name[20];
    Hours departure; // Saída da Origem
    Hours arrival;   // Chegada no Destino
    int found;       // 1 se achou, 0 se não
} RouteResult;

/// @brief Inicializa a estrutura principal, criando a lista de linhas.
/// @param data Ponteiro para a estrutura principal.
/// @return 1 se inicializar corretamente, 0 caso contrário.
int defineObject(Data *data);

/// @brief Carrega todas as linhas e paradas a partir dos arquivos existentes.
/// @param data Estrutura que receberá os dados carregados.
/// @return 1 se carregar corretamente, 0 caso contrário.
int loadData(Data *data);

/// @brief Salva todas as linhas e paradas em arquivos texto e binários.
/// @param data Estrutura que contém os dados a serem salvos.
/// @return 1 se salvar corretamente, 0 caso contrário.
int saveObject(Data *data);

/// @brief Libera toda a memória alocada pela estrutura principal.
/// @param data Estrutura a ser destruída.
/// @return 1 se liberar corretamente, 0 caso contrário.
int deleteObject(Data *data);

/// @brief Insere uma nova linha de ônibus na estrutura.
/// @param data Estrutura onde a linha será inserida.
/// @param name Nome da linha (wide char).
/// @param enterprise Empresa responsável (wide char).
/// @return 1 se inserir corretamente, 0 caso contrário.
int insertBusLine(Data *data, wchar_t *name, wchar_t *enterprise);

/// @brief Verifica se uma linha de ônibus já existe na estrutura.
/// @param data Estrutura a ser verificada.
/// @param name Nome da linha procurada (wide char).
/// @return 1 se a linha existir, 0 caso contrário.
int hasBusLine(Data *data, wchar_t *name);

/// @brief Insere uma parada em uma linha após um nó especificado
/// @param line A linha
/// @param prev_node O nó especificado
/// @param new_data A nova parada
/// @note Se a linha estiver vazia, então é inserido como a primeira parada
/// @return 1 se sucesso e 0 se falha
int insertStopAfter(BusLine *line, DoubleLinkedListNode *prev_node, BusStop *new_data);

/// @brief Remove um nó de parada de uma linha
/// @param line A linha
/// @param target_node O nó alvo
/// @return 1 se sucesso e 0 se falha
int removeStopNode(BusLine *line, DoubleLinkedListNode *target_node);

/// @brief Remove o nó de linha da lista de linhas
/// @param data Os dados
/// @param target_node O nó alvo
/// @return 1 se sucesso e 0 se falha
int removeLineNode(Data *dados, SimpleLinkedListNode *target_node);

/// @brief Cria um vetor com de nós de uma linha com nomes parecidos com um termo de busca
/// @param list_to_search O nó de linha para a busca
/// @param array_out O vetor de saída
/// @param array_length O tamanho desse vetor
/// @param search_term O termo de busca
/// @return 1 se sucesso e 0 se falha
int get_stops_to_array(SimpleLinkedListNode *list_to_search, DoubleLinkedListNode ***array_out, int *array_length, wchar_t *search_term);

/// @brief Cria um vetor com todos os nós de paradas de todas as linhas com nomes parecidos com um termo de busca
/// @param data Os dados
/// @param array_out O vetor de nós de saída
/// @param array_length O tamanho desse vetor
/// @param search_term O termo de busca
/// @return 1 se sucesso e 0 se falha
int get_all_stops_to_array(Data *data, DoubleLinkedListNode ***array_out, int *array_length, wchar_t *search_term);

/// @brief Cria um vetor de nós de linhas com nomes parecidos com o termo de busca
/// @param data Os dados
/// @param array_out O vetor de saída
/// @param array_length O tamanho desse vetor
/// @param search_term O termo de busca
/// @return 1 se sucesso e 0 se falha
int get_lines_to_array(Data *data, SimpleLinkedListNode ***array_out, int *array_length, wchar_t *search_term);

/// @brief Cria uma lista de strings com o nome, horário de saída e chegada de uma parada de um vetor de nó de paradas
/// @param node_array O vetor de nós
/// @param length O tamanho do vetor
/// @return Retorna a lista de strings ou NULL caso falhe
wchar_t **create_stop_strings(DoubleLinkedListNode **node_array, int length);

/// @brief Cria uma lista com os nomes das linhas, empresa e estado
/// @param node_array O vetor de nós de linhas
/// @param length O tamanho do vetor
/// @return Retorna a lista de strings ou NULL se falhar
wchar_t **create_line_strings(SimpleLinkedListNode **node_array, int length);

/// @brief Procura a melhor rota dado dois nomes de parada e um horário de chegada alvo
/// @param data Os dados
/// @param origin_name O nome da origem
/// @param dest_name O nome do destino
/// @param target_arrival O horário de chegada alvo
/// @return O resultado da busca
RouteResult find_best_route(Data *data, wchar_t *origin_name, wchar_t *dest_name, Hours target_arrival);

/// @brief Cria uma lista de paradas unicas
/// @param data Os dados
/// @param out_length O tamanho de saída
/// @param search_term O termo de busca
/// @return Retorna a lista de strings ou NULL caso falha
wchar_t **create_unique_names_list(Data *data, int *out_length, wchar_t *search_term);

#endif
