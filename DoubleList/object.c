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
    
    // O loop original apenas percorria sem liberar o conteudo 'interno' (BusLine).
    // Assumindo que destroy_sll cuida dos nós da lista simples.
    // Se precisar liberar a lista dupla dentro de BusLine, faça aqui:
    while (curr)
    {
        BusLine *DL = ((BusLine*)curr->info);
        // Exemplo: deleteLine(DL->list); free(DL->list); free(DL);
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

// Funções expostas para os handles
int deleteLine(DoubleLinkedList *dl) {
    if (!dl || !dl->head) return 0;

    DoubleLinkedListNode *curr = dl->head;
    DoubleLinkedListNode *next;

    do {
        next = curr->next;
        
        free(curr); // Libera o nó
        // Nota: Se 'info' (DataType) for um ponteiro alocado, precisa de free(curr->info) antes
        
        curr = next;
    } while (curr != dl->head); // Assumindo lista circular baseado no loop original

    dl->head = NULL;
    dl->size = 0;

    return 1;
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

// Argumentos char mudaram para wchar_t
int insertBusStop(Object *obj, wchar_t *line_number, wchar_t *name, Hours departure_time, Hours arrival_time)
{
    if (!obj || !obj->SLL) return 0;

    BusStop *stop = malloc(sizeof(BusStop));
    if (!stop) return 0;

    // wcscpy substitui strcpy
    wcscpy(stop->nome, name);
    stop->arrival_time = arrival_time;
    stop->departure_time = departure_time;

    SimpleLinkedListNode *curr = obj->SLL->head;

    while (curr) {
        BusLine *line = (BusLine*) curr->info;

        // wcscmp substitui strcmp
        if (line && !wcscmp(line->name, line_number)) {

            if (!add(line->list, stop)) {
                free(stop);
                return 0;
            }

            return 1;
        }

        curr = curr->next;
    }

    free(stop);
    return 0;
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
                    if (!best_route.found || current_diff < min_diff) 
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

                                if (departure_min < arrival_min) {
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
        {
            return NULL; 
        }

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

        if (!string_array[i]) {
            return NULL; 
        }

        wchar_t *status_text;
        if (line->list == NULL || line->list->head == NULL || line->list->size < 1) {
            status_text = L"(Em Aberto)"; // Sem paradas cadastradas ou com uma única parada
        } else {
            status_text = L"(Ativa)";
        }

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