/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef FILE_MENAGER_H
#define FILE_MENAGER_H

#define HANDLE_NAME "FileHandle.txt"  //Nome do fileHandle

#include "simpleLinkedList.h"
#include "doubleList.h"
#include <stdio.h>
#include <string.h>
#include <object.h>

/// @brief Carrega todas as linhas e paradas a partir dos arquivos salvos.
/// @param object Estrutura principal que receberá os dados carregados.
/// @return 1 se carregar com sucesso, 0 caso contrário.
int loadFromFile(Data *object);

/// @brief Salva todas as linhas e paradas em arquivos texto e binários.
/// @param object Estrutura principal contendo os dados.
/// @return 1 se salvar com sucesso, 0 caso contrário.
int saveToFile(Data *object);

/// @brief Garante a existência do arquivo principal e o abre para leitura/escrita.
/// @param file Ponteiro que receberá o arquivo aberto.
/// @return 1 se o arquivo existir ou for criado, 0 caso contrário.
int open_line_file(FILE **file);

/// @brief Lê o arquivo principal e cria as linhas dentro da estrutura.
/// @param object Estrutura onde as linhas serão inseridas.
/// @param file Handle do arquivo já aberto.
/// @return 1 se as linhas forem carregadas corretamente, 0 caso contrário.
int get_lines_from_file(Data *object, FILE *file);

/// @brief Abre o arquivo binário de uma linha e carrega suas paradas.
/// @param str Nome da linha (wide char).
/// @param line Estrutura que receberá as paradas.
/// @return 1 se abrir/carregar corretamente, 0 caso contrário.
int openLine(wchar_t *str, BusLine *line);

/// @brief Salva todas as paradas de uma linha em um arquivo binário.
/// @param dl Lista de paradas.
/// @param path Caminho do arquivo binário (char*).
/// @return 1 se salvar corretamente, 0 caso contrário.
int saveStops(DoubleLinkedList *dl, char *path);

/// @brief Remove o arquivo binário associado a uma linha.
/// @param path Nome da linha (wide char).
/// @return 1 se o arquivo for removido, 0 caso contrário.
int removeLine(wchar_t *path);

/// @brief Salva todas as linhas no arquivo principal e cria seus arquivos .dat.
/// @param obj Estrutura contendo todas as linhas.
/// @return 1 se salvar corretamente, 0 caso contrário.
int saveLine(Data *obj);


#endif
