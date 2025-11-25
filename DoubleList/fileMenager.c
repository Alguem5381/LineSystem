#include "fileMenager.h"
#include "doubleList.h"
#include <string.h>

int comp(BusStop *data1, BusStop *data2) {
  if (strcmp(data1->nome, data2->nome))
    return 1;
  return 0;
}

int init(Object *object)
{
    FILE *file;
    if(!openFileHandle(&file))
        return 0;

    if(object->SLL != NULL){
        return 0;
    }

    linkedList *LL = (linkedList*)malloc(sizeof(linkedList));
    if (!LL)
        return 0;

    object->SLL = LL;
    LL->head = NULL;
    handleManeger(object, file);
    fclose(file);
    return 1;
}

int openFileHandle(FILE **file){
    printf("4\n");
    *file = fopen(HANDLE_NAME, "r");

    if(!*file)
    {
        printf("5\n");
        *file = fopen(HANDLE_NAME, "w+");

        if (!*file){
            printf("6\n");
            return 0;}
        else
            return 1;
    }
    return 1;
}

int handleManeger(Object *object, FILE *file){
    char buffer[255];
    BusLine *temp;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        temp = (BusLine*)malloc(sizeof(BusLine));

        char *name = strtok(buffer, ",");
        char *enterprise = strtok(NULL, ",");

        strncpy(temp->name, name, sizeof(temp->name)-1);
        temp->name[sizeof(temp->name)-1] = '\0';

        strncpy(temp->enterprise, enterprise, sizeof(temp->enterprise)-1);
        temp->enterprise[sizeof(temp->enterprise)-1] = '\0';

        openLine(name, temp);
        initInsert(object->SLL, temp);
    }
    return 1;
}

int openLine(char *str, BusLine *list){
    char line[50] = "lines/";
    strcat(line, str);
    strcat(line, ".dat");

    FILE *file = fopen(line, "rb");

    if(!file){
        printf("%s nao exite.", line);
        file = fopen(line, "w");
        if (!file)
            return 0;
    }
    list->list = malloc(sizeof(DoubleList));
    create(list->list);

    BusStop temp;
    while (fread(&temp, sizeof(BusStop), 1, file) == 1)
    {
         BusStop *novo = malloc(sizeof(BusStop));
        *novo = temp;

        add(list->list, novo);
    }

    fclose(file);
    return 1;
}

int saveObject(Object *obj){
    printf("1\n");
    if(!obj)
        return 0;
    if(!saveLine(obj->SLL)){
        printf("Erro ao salvar.");
        return 0;
    }
    return 1;
}

int saveLine(linkedList *ll){
    printf("2\n");
    FILE *file = fopen(HANDLE_NAME, "w+");

    if(!file){
        perror("Erro fopen w+");
        printf("HANDLE_NAME = '%s'\n", HANDLE_NAME);
        printf("3\n");
        if(!openFileHandle(&file))
        {
            printf("Opa, to aqui.");
            return 0;
        }
        else
            file = fopen(HANDLE_NAME, "w+");
    }
    SLL_Node *curr = ll->head;
    while(curr)
    {
        char line[50] = "lines/";
        BusLine *DL = ((BusLine*)curr->info);
        fprintf(file,"%s,%s\n", DL->name, DL->enterprise);
        strcat(line, DL->name);
        strcat(line, ".dat");
        saveStops(DL->list, line);
        curr = curr->next;
    }
    fclose(file);
    return 1;
}

int saveStops(DoubleList *dl, char *path){

    if(!dl)
        return 0;

    FILE *file = fopen(path, "wb");

    if(!file)
    {
        printf("%s nao exite.", path);
            return 0;
    }

    Node *curr = dl->head;
    int i= 0;

    while (i < dl->size)
    {
        fwrite(curr->info, sizeof(DataType), 1, file);
        curr = curr->next;
        i++;
    }

    fclose(file);
    return 1;
}

int removeLine(char *path){
    char line[50] = "lines/";
    strcat(line, path);
    if (remove(line) == 0) {
        printf("Arquivo binário deletado!\n");
        return 1;
    } else {
        perror("Erro ao deletar arquivo binário");
        return 0;
    }

}
