/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#include "fileMenager.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>
#include <stdlib.h>

// Retorna 1 se sucesso, 0 se falha
int wide_path_to_multibyte(const wchar_t *wide_path, char *mb_path, size_t max_size) {
    size_t res = wcstombs(mb_path, wide_path, max_size);
    return (res != (size_t)-1);
}

int loadFromFile(Object *object)
{
    if (!object) return 0;

    FILE *file;
    if (!open_line_file(&file))
        return 0;
    
    // Agora usando a versão otimizada
    if (!get_lines_from_file(object, file)) {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int open_line_file(FILE **file) {
    // HANDLE_NAME geralmente é um define string literal. 
    // O sistema de arquivos espera char*, então mantemos assim.
    *file = fopen(HANDLE_NAME, "r"); // Tenta abrir leitura
    
    if (!*file) {
        // Se falhar, cria novo. Nota: "w+" trunca o arquivo.
        *file = fopen(HANDLE_NAME, "w+");
        if (!*file) return 0;
    }
    return 1;
}

int get_lines_from_file(Object *object, FILE *file) {
    wchar_t buffer[1024]; // Buffer WIDE
    wchar_t *context = NULL; // Necessário para wcstok (thread-safe)

    // fgetws substitui fgets
    while (fgetws(buffer, 1024, file)) {
        // Remove quebra de linha (\r ou \n)
        buffer[wcscspn(buffer, L"\r\n")] = L'\0';

        // 1. Tokenização (Primeira passagem para validar)
        // Nota: wcstok precisa do endereço de 'context'
        wchar_t *name = wcstok(buffer, L",", &context);
        wchar_t *enterprise = wcstok(NULL, L",", &context);

        // Se a linha estiver quebrada, pula sem alocar memória (OTIMIZAÇÃO)
        if (!name || !enterprise) {
            continue; 
        }

        // 2. Alocação (Só agora que sabemos que os dados existem)
        BusLine *temp = malloc(sizeof(BusLine));
        if (!temp) return 0;

        // 3. Cópia segura usando swprintf (substitui strncpy e garante \0)
        swprintf(temp->name, sizeof(temp->name)/sizeof(wchar_t), L"%ls", name);
        swprintf(temp->enterprise, sizeof(temp->enterprise)/sizeof(wchar_t), L"%ls", enterprise);

        // Funções da lista devem esperar wchar_t agora
        if (!openLine(name, temp) || !init_insert_sll(object->SLL, temp)) {
            free(temp);
            return 0;
        }
    }

    return 1;
}

// str agora deve ser wchar_t*
int openLine(wchar_t *str, BusLine *list){
    wchar_t wide_path[100];
    char mb_path[100]; // Path "multibyte" para o sistema operacional

    // Formata string larga interna: lines/Nome.dat
    if(swprintf(wide_path, 100, L"lines/%ls.dat", str) < 0)
        return 0;

    // CONVERSÃO: O sistema de arquivos (fopen/mkdir) não entende wchar_t nativamente no Linux
    if (!wide_path_to_multibyte(wide_path, mb_path, sizeof(mb_path))) return 0;

    // mkdir usa string comum
    int result = mkdir("lines", 0777); 
    if (result && errno != EEXIST)
        return 0;

    FILE *file = fopen(mb_path, "rb");
    
    if(!file){
        file = fopen(mb_path, "wb+");
        if (!file)
            return 0;
    }

    list->list = malloc(sizeof(DoubleLinkedList));
    if (!list->list)
    {
        fclose(file);
        return 0;
    }

    create(list->list);

    BusStop temp;
    // fread lê binário, funciona igual desde que a struct BusStop tenha sido atualizada
    // Se BusStop agora tem wchar_t, o tamanho lido será maior, o que é correto.
    while (fread(&temp, sizeof(BusStop), 1, file) == 1){

        BusStop *novo = malloc(sizeof(BusStop));
        if (!novo){
            fclose(file);
            return 0;
        }

        *novo = temp;

        if(!append(list->list, novo)){
            free(novo);
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

int saveToFile(Object *obj){
    if(!obj || !saveLine(obj))
        return 0;
    return 1;
}

int saveLine(Object *obj){
    FILE *file = fopen(HANDLE_NAME, "w+");

    if(!file)
        return 0;

    SimpleLinkedListNode *current = obj->SLL->head;
    while(current)
    {
        wchar_t wide_path[100];
        char mb_path[100];

        BusLine *double_linked_list = ((BusLine*)current->info);
        if(!double_linked_list) continue;
        
        // Formata o path em wide
        if(swprintf(wide_path, 100, L"lines/%ls.dat", double_linked_list->name) < 0)
            continue;
        
        // Converte path para char para abrir arquivo binário
        if (!wide_path_to_multibyte(wide_path, mb_path, sizeof(mb_path))) continue;

        // Escreve no arquivo principal usando fwprintf (Suporte a Wide Char no texto)
        // %ls imprime string wide
        fwprintf(file, L"%ls,%ls\n", double_linked_list->name, double_linked_list->enterprise);
        
        // Passa o path já convertido (char*) pois saveStops usa fopen
        saveStops(double_linked_list->list, mb_path);
        
        current = current->next;
    }
    fclose(file);
    return 1;
}

int saveStops(DoubleLinkedList *dl, char *path){
    if(!dl|| !path)
        return 0;
    
    FILE *file = fopen(path, "wb+");

    if(!file)
        return 0;

    DoubleLinkedListNode *curr = dl->head;
    
    for (int i = 0; i < dl->size; i++)
    {
        fwrite(curr->info, sizeof(DataType), 1, file);
        curr = curr->next;
    }

    fclose(file);
    return 1;
}

int removeLine(wchar_t *path){
    if (!path) return 0;

    wchar_t wide_full_path[100];
    char mb_full_path[100];

    // Cria lines/Nome.dat em wide
    if (swprintf(wide_full_path, 100, L"lines/%ls.dat", path) < 0)
        return 0;

    // Converte para char para usar remove()
    if (!wide_path_to_multibyte(wide_full_path, mb_full_path, sizeof(mb_full_path)))
        return 0;

    return remove(mb_full_path) == 0;
}