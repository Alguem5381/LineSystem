/* Júlio Cesar lima de Souza
 * Rodrigo Marques Cabral
 * Raul Vilela
*/
#ifndef FILE_MENAGER_H
#define FILE_MENAGER_H

#define HANDLE_NAME "FileHandle.txt"

#include "simpleLinkedList.h"
#include "doubleList.h"
#include <stdio.h>
#include <string.h>
#include <object.h>

// Funções do fileMenager
int loadFromFile(Data *object);
int saveToFile(Data *object);

int open_line_file(FILE **file);
int get_lines_from_file(Data *object, FILE *file);
int openLine(wchar_t *str, BusLine *line);
int saveStops(DoubleLinkedList *dl, char *path);
int removeLine(wchar_t *path);
int saveLine(Data *obj);

#endif
