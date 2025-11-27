/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include "object.h"
#include "fileMenager.h"
#include <wchar.h>   // Necessário para wcscpy, wcscmp
#include <stdlib.h>  // Necessário para malloc, free
#include <letter.h>
#include <math.h>

// Função de definição e destruição
int defineObject(Object *object)
{
    if (!object) return 0;

    SimpleLinkedList *LL = malloc(sizeof(SimpleLinkedList));
    if (!LL) return 0;

    define_sll(LL);
    object->SLL = LL;

    return 1;
}

int deleteObject(Object *obj){
    if (!obj || !obj->SLL) return 0;

    SimpleLinkedListNode *curr = obj->SLL->head;

    while (curr)
    {
        BusLine *DL = ((BusLine*)curr->info);
        curr = curr->next;
    }
    destroy_sll(obj->SLL);

    return 1;
}

// Função de carregamento e descarregamento
int loadData(Object *object) {
    if (!object) return 0;
    return loadFromFile(object);  
}

int saveObject(Object *object) {
    if (!object || !object->SLL) return 0;
    return saveToFile(object); 
}

// Argumentos char mudaram para wchar_t
int insertBusLine(Object *obj, wchar_t *name, wchar_t *enterprise)
{
    if (hasBusLine(obj, name))
        return 0;

    BusLine *line = malloc(sizeof(BusLine));

    if(!line)
        return 0;

    DoubleLinkedList *double_linked_list = malloc(sizeof(DoubleLinkedList));

    if(!double_linked_list)
    {
        free(line);
        return 0;
    }

    create(double_linked_list);

    // wcscpy substitui strcpy
    wcscpy(line->enterprise, enterprise);
    wcscpy(line->name, name);

    double_linked_list->head = NULL;
    double_linked_list->size = 0;

    line->list = double_linked_list;

    if(!init_insert_sll(obj->SLL, line))
    {
        free(double_linked_list);
        free(line);
        return 0;
    }

    return 1;
}

// Argumento char mudou para wchar_t
int hasBusLine(Object *obj, wchar_t *name){
    SimpleLinkedListNode *curr = obj->SLL->head;
    while (curr)
    {
        // Comparação wide
        if (!wcscmp((((BusLine*)(curr->info))->name), name))
            return 1;
        curr = curr->next;
    }
    return 0;
}

RouteResult find_best_route(Object *obj, wchar_t *origin_name, wchar_t *dest_name, Hours target_arrival) {
    RouteResult best_route;
    best_route.found = 0;
    
    int target_min = to_minutes(target_arrival);
    int min_diff = 100000; // Valor alto inicial

    if (!obj || !obj->SLL) return best_route;

    SimpleLinkedListNode *line_node = obj->SLL->head;

    // Percorre as Linhas
    while (line_node) {
        BusLine *line = (BusLine*)line_node->info;
        
        if (line && line->list && line->list->head) 
        {
            DoubleLinkedListNode *dest_node = line->list->head;
            DoubleLinkedListNode *start_check = dest_node;

            // Procura todas as ocorrências do destino
            do 
            {
                BusStop *stop_dest = (BusStop*)dest_node->info;

                // Encontra um possível destino
                if (wcscmp(stop_dest->nome, dest_name) == 0) 
                {
                    // Calcula a diferença de tempo para saber se vale a pena investigar
                    int arrival_min = to_minutes(stop_dest->arrival_time);
                    int current_diff = abs(target_min - arrival_min);

                    // Só gastamos processamento procurando a origem se esse destino
                    // for potencialmente melhor que o que já temos.
                    // ou se ainda não tem a melhor rota.
                    if ((!best_route.found || current_diff < min_diff) && arrival_min >= target_min)
                    {
                        // Procura a origem para trás
                        DoubleLinkedListNode *curr_back = dest_node->prev;
                        int route_found_in_segment = 0;

                        // Retrocede até dar a volta completa
                        while (curr_back != dest_node) 
                        {
                            BusStop *stop_origin = (BusStop*)curr_back->info;

                            // Achou uma origem
                            if (wcscmp(stop_origin->nome, origin_name) == 0) 
                            {
                                // O ônibus precisa passar na origem antes do destino
                                int departure_min = to_minutes(stop_origin->departure_time);

                                if (departure_min < arrival_min) 
                                {
                                    // Achamos uma rota válida e melhor que a anterior
                                    min_diff = current_diff;

                                    best_route.found = 1;
                                    wcscpy(best_route.line_name, line->name);
                                    wcscpy(best_route.enterprise, line->enterprise);
                                    wcscpy(best_route.origin_name, origin_name);
                                    wcscpy(best_route.dest_name, dest_name);
                                    best_route.departure = stop_origin->departure_time;
                                    best_route.arrival = stop_dest->arrival_time;

                                    route_found_in_segment = 1;
                                }
                                // Se departure > arrival, significa que a origem está depois
                                // do destino na linha temporal, então continuamos
                                // voltando para ver se achamos uma origem anterior válida.
                                
                                if (route_found_in_segment) break; 
                            }
                            curr_back = curr_back->prev;
                        }
                    }
                }

                dest_node = dest_node->next;
            } while (dest_node != start_check);
        }
        line_node = line_node->next;
    }

    return best_route;
}

//Busca os nós das linhas
int get_lines_to_array(Object *obj, SimpleLinkedListNode ***array_out, int *array_length, wchar_t *search_term)
{
    if (!obj || !array_out || !array_length)
        return 0;

    int capacity = 2;
    int count = 0;

    *array_out = malloc(capacity * sizeof(SimpleLinkedListNode*));

    if (!*array_out)
    {
        *array_length = 0;
        return 0;
    }

    SimpleLinkedListNode *node = obj->SLL->head;

    while (node)
    {
        BusLine *line = (BusLine*)node->info;

        // Se o filtro for NULL ou vazio, pega tudo. Se não, verifica.
        if (search_term && wcslen(search_term) > 0) {
            if (!contains_wstr(search_term, line->name))
            {
                node = node->next;
                continue;
            }
        }

        // Verifica se precisa expandir
        if (count >= capacity)
        {
            capacity *= 2;
            
            // Realocação
            SimpleLinkedListNode **temp = realloc(*array_out, capacity * sizeof(SimpleLinkedListNode*));

            if (temp == NULL)
            {
                free(*array_out); // Libera o antigo para não vazar
                *array_out = NULL;
                *array_length = 0;
                return 0;
            }
            *array_out = temp;
        }

        // Coloco o ponteiro no array
        (*array_out)[count] = node;

        count++;
        node = node->next;
    }

    *array_length = count;
    return 1; // Sucesso
}

int get_all_stops_to_array(Object *obj, DoubleLinkedListNode ***array_out, int *array_length, wchar_t *search_term)
{
    if (!obj || !obj->SLL || !array_out || !array_length)
        return 0;

    int capacity = 16; // Começa maior por causa da quantidade
    int count = 0;

    // Aloca o array
    *array_out = malloc(capacity * sizeof(DoubleLinkedListNode*));
    if (!*array_out) 
    {
        *array_length = 0;
        return 0;
    }

    SimpleLinkedListNode *line_node = obj->SLL->head;

    while (line_node)
    {
        BusLine *line = (BusLine*)line_node->info;

        // Verifica se a linha é válida e se tem lista de paradas
        if (line && line->list && line->list->head)
        {
            // Percorre as Paradas
            DoubleLinkedListNode *stop_node = line->list->head;
            DoubleLinkedListNode *start_node = stop_node; // Para controle da lista circular

            do
            {
                BusStop *stop = (BusStop*)stop_node->info;
                int match = 1;

                // Verificação do Filtro (Search Term)
                if (search_term && wcslen(search_term) > 0) {
                    if (!contains_wstr(search_term, stop->nome)) 
                        match = 0;
                }

                // Se passou no filtro, adiciona ao array
                if (match)
                {
                    // Verifica expansão
                    if (count >= capacity)
                    {
                        capacity *= 2;
                        DoubleLinkedListNode **temp = realloc(*array_out, capacity * sizeof(DoubleLinkedListNode*));

                        if (!temp) {
                            // Libera tudo que já foi pego
                            free(*array_out);
                            *array_out = NULL;
                            *array_length = 0;
                            return 0;
                        }
                        *array_out = temp;
                    }

                    // Adiciona
                    (*array_out)[count] = stop_node;
                    count++;
                }

                stop_node = stop_node->next;

            } while (stop_node != start_node); // Fim do loop das paradas
        }

        line_node = line_node->next; // Próxima linha
    }

    *array_length = count;
    return 1;
}

int get_stops_to_array(SimpleLinkedListNode *list_to_search, DoubleLinkedListNode ***array_out, int *array_length, wchar_t *search_term)
{
    if (!list_to_search || !array_out || !array_length)
        return 0;

    int capacity = 2;
    int count = 0;

    *array_out = malloc(capacity * sizeof(DoubleLinkedListNode*));

    if (!*array_out)
    {
        *array_length = 0;
        return 0;
    }

    // Faz o cast
    BusLine *line = (BusLine*)list_to_search->info;

    // Nós para iterar na lista duplamente encadeada
    DoubleLinkedListNode *node = line->list->head;
    DoubleLinkedListNode *start = node;

    do
    {
        // Caso a linha não tenha paradas
        if (!node) break;

        BusStop *stop = node->info;

        // Se o filtro for NULL ou vazio, pega tudo. Se não, verifica.
        if (search_term && wcslen(search_term) > 0) {
            if (!contains_wstr(search_term, stop->nome))
            {
                node = node->next;
                continue;
            }
        }

        // Verifica se precisa expandir
        if (count >= capacity)
        {
            capacity *= 2;
            
            // Realocação
            DoubleLinkedListNode **temp = realloc(*array_out, capacity * sizeof(DoubleLinkedListNode*));

            if (temp == NULL)
            {
                free(*array_out); // Libera o antigo para não vazar
                *array_out = NULL;
                *array_length = 0;
                return 0;   //Falha
            }
            *array_out = temp;
        }

        // Coloco o ponteiro no array
        (*array_out)[count] = node;

        count++;
        node = node->next;
    }
    while (node != start);

    *array_length = count;
    return 1; // Sucesso
}

//Auxiliar
void clear_stops_list(DoubleLinkedList *dl) 
{
    if (!dl || !dl->head) return;

    DoubleLinkedListNode *curr = dl->head;
    DoubleLinkedListNode *next;

    DoubleLinkedListNode *start = dl->head;

    do 
    {
        next = curr->next;
        
        // Libera o conteúdo
        if (curr->info) {
            free(curr->info); 
        }
        
        // Libera o nó
        free(curr);
        
        curr = next;
    } while (curr != start);

    dl->head = NULL;
    dl->size = 0;
}

// Remove um nó específico da lista de Linhas
int removeLineNode(Object *obj, SimpleLinkedListNode *target_node)
{
    if (!obj || !obj->SLL || !obj->SLL->head || !target_node) 
        return 0;

    SimpleLinkedListNode *curr = obj->SLL->head;
    SimpleLinkedListNode *prev = NULL;

    while (curr) 
    {
        if (curr == target_node)
        {
            BusLine *line = (BusLine*)curr->info;

            removeLine(line->name);

            if (line) {
                // Limpa a lista de paradas dessa linha
                if (line->list) 
                {
                    clear_stops_list(line->list);
                    free(line->list);
                }
                free(line); // Libera a struct BusLine
            }

            if (prev == NULL) 
                obj->SLL->head = curr->next;
            else 
                prev->next = curr->next;

            free(curr); // Libera o nó da lista simples
            return 1;
        }

        prev = curr;
        curr = curr->next;
    }

    return 0; // Nó não pertence a esta lista
}

wchar_t **create_stop_strings(DoubleLinkedListNode **node_array, int length)
{
    if (!node_array || length <= 0) return NULL;

    wchar_t **string_array = malloc(length * sizeof(wchar_t*));
    if (!string_array) return NULL;

    for (int i = 0; i < length; i++)
    {
        BusStop *stop = (BusStop*)node_array[i]->info;

        int buffer_size = 128;
        string_array[i] = malloc(buffer_size * sizeof(wchar_t));

        if (!string_array[i]) 
            return NULL; 

        swprintf(string_array[i], buffer_size, 
            L"%-20ls Saída: %02dh%02dm | Chegada: %02dh%02dm", 
            stop->nome, 
            stop->departure_time.hours, stop->departure_time.minutes,
            stop->arrival_time.hours, stop->arrival_time.minutes
        );
    }

    return string_array;
}

wchar_t **create_line_strings(SimpleLinkedListNode **node_array, int length)
{
    if (!node_array || length <= 0) return NULL;

    wchar_t **string_array = malloc(length * sizeof(wchar_t*));
    if (!string_array) return NULL;

    for (int i = 0; i < length; i++)
    {
        BusLine *line = (BusLine*)node_array[i]->info;
        
        int buffer_size = 128;
        string_array[i] = malloc(buffer_size * sizeof(wchar_t));

        if (!string_array[i]) 
            return NULL;

        wchar_t *status_text;
        if (line->list == NULL || line->list->head == NULL || line->list->size < 1) 
            status_text = L"(Em Aberto)"; // Sem paradas cadastradas ou com uma única parada
        else 
            status_text = L"(Ativa)";

        swprintf(string_array[i], buffer_size, 
            L"%-20ls | %-20ls | %ls", 
            line->name, 
            line->enterprise, 
            status_text
        );
    }

    return string_array;
}

int removeStopNode(BusLine *line, DoubleLinkedListNode *target_node)
{
    // Validações de segurança
    if (!line || !line->list || !target_node) 
        return 0;

    DoubleLinkedList *dl = line->list;

    if (target_node->next == target_node) 
    {
        dl->head = NULL;
        dl->size = 0;
    } 
    else 
    {
        target_node->prev->next = target_node->next;
        target_node->next->prev = target_node->prev;

        if (dl->head == target_node) 
            dl->head = target_node->next;

        dl->size--;
    }

    if (target_node->info) 
        free(target_node->info);

    free(target_node);

    return 1;
}

// Insere uma nova parada após o nó de referência.
// Se prev_node for NULL, insere no início ou cria a lista se estiver vazia.
int insertStopAfter(BusLine *line, DoubleLinkedListNode *prev_node, BusStop *new_data)
{
    // Validações
    if (!line || !line->list || !new_data) return 0;

    DoubleLinkedListNode *new_node = malloc(sizeof(DoubleLinkedListNode));
    if (!new_node) return 0;

    new_node->info = new_data;

    DoubleLinkedList *dl = line->list;

    if (dl->head == NULL) 
    {
        new_node->next = new_node;
        new_node->prev = new_node;

        dl->head = new_node;
    }
    else if (prev_node != NULL) 
    {
        new_node->prev = prev_node;
        new_node->next = prev_node->next;

        prev_node->next->prev = new_node;
        prev_node->next = new_node;
    }
    else
        return 0;

    dl->size++;
    return 1;
}

//Auxiliar
int compare_wide_strings(const void *a, const void *b) 
{
    const wchar_t **str_a = (const wchar_t **)a;
    const wchar_t **str_b = (const wchar_t **)b;
    return wcscmp(*str_a, *str_b);
}

// Retorna um array de strings unicas
wchar_t **create_unique_names_list(Object *data, int *out_length, wchar_t *search_term)
{
    int capacity = 0;
    
    // Conta total de paradas no sistema
    SimpleLinkedListNode *line_node = data->SLL->head;
    while (line_node) 
    {
        BusLine *line = (BusLine*)line_node->info;
        if (line && line->list) capacity += line->list->size;
        line_node = line_node->next;
    }

    if (capacity == 0) 
    {
        *out_length = 0;
        return NULL;
    }

    // Aloca vetor temporário
    wchar_t **temp_names = malloc(capacity * sizeof(wchar_t*));
    int count = 0;

    // Coleta os ponteiros dos nomes
    line_node = data->SLL->head;
    while (line_node) 
    {
        BusLine *line = (BusLine*)line_node->info;
        if (line && line->list) 
        {
            DoubleLinkedListNode *stop_node = line->list->head;
            DoubleLinkedListNode *start = stop_node;
            if (stop_node) 
            {
                do 
                {
                    BusStop *stop = (BusStop*)stop_node->info;
                    
                    // Aplica o filtro de texto se ele existir
                    int match = 1;
                    if (search_term && wcslen(search_term) > 0)
                         if (!wcsstr(stop->nome, search_term)) match = 0; // Se não contem, ignora

                    if (match) 
                        temp_names[count++] = stop->nome; // Guarda só o ponteiro
                    
                    stop_node = stop_node->next;
                } while (stop_node != start);
            }
        }
        line_node = line_node->next;
    }

    // Nenhum tinha nomes parecidos
    if (count == 0) 
    {
        free(temp_names);
        *out_length = 0;
        return NULL;
    }

    // Ordena
    qsort(temp_names, count, sizeof(wchar_t*), compare_wide_strings);

    // Filtra
    wchar_t **unique_list = malloc(count * sizeof(wchar_t*));
    int unique_count = 0;

    if (count > 0) {
        // Aponta para o endereço original do primeiro
        unique_list[0] = temp_names[0]; 
        unique_count++;

        for (int i = 1; i < count; i++) 
        {
            // Se for diferente do anterior
            if (wcscmp(temp_names[i], temp_names[i-1]) != 0) 
            {
                unique_list[unique_count] = temp_names[i];
                unique_count++;
            }
        }
    }

    free(temp_names); // Libera o vetor temporário de coleta
    *out_length = unique_count;

    return unique_list;
}