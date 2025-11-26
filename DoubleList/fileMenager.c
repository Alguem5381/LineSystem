#include "fileMenager.h"
#include "object.h"


int loadFromFile(Object *object)
{
    if (!object) return 0;

    FILE *file;
    if (!openFileHandle(&file))
        return 0;

    
    if (!handleManeger(object, file)) {      
        object->SLL = NULL;
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;

}

int openFileHandle(FILE **file) {
    *file = fopen(HANDLE_NAME, "r");
    if (!*file) {
        *file = fopen(HANDLE_NAME, "w+");
        if (!*file) return 0;
    }
    return 1;
}


int handleManeger(Object *object, FILE *file) {
    char buffer[255];
    BusLine *temp;

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        temp = malloc(sizeof(BusLine));
        if (!temp) return 0;

        char *name = strtok(buffer, ",");
        char *enterprise = strtok(NULL, ",");

        if (!name || !enterprise) {
            free(temp);
            continue; 
        }

        strncpy(temp->name, name, sizeof(temp->name)-1);
        temp->name[sizeof(temp->name)-1] = '\0';

        strncpy(temp->enterprise, enterprise, sizeof(temp->enterprise)-1);
        temp->enterprise[sizeof(temp->enterprise)-1] = '\0';

        if (!openLine(name, temp) || !initInsert(object->SLL, temp)) {
            free(temp);
            return 0;
        }
    }
    return 1;
}

int openLine(char *str, BusLine *list){
    char line[50];
    if(snprintf(line, sizeof(line), "lines/%s.dat", str)>= sizeof(line))
        return 0;

    FILE *file = fopen(line, "rb");
    
    if(!file){
        file = fopen(line, "wb+");
        if (!file)
            return 0;
    }

    list->list = malloc(sizeof(DoubleList));
    if (!list->list)
    {
        fclose(file);
        return 0;
    }
    
    create(list->list);

    BusStop temp;
    while (fread(&temp, sizeof(BusStop), 1, file) == 1){

         BusStop *novo = malloc(sizeof(BusStop));
        if (!novo){
            
            fclose(file);
            return 0;
        }
        
        *novo = temp;

        if(!add(list->list, novo)){
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

int saveLine(Object *ll){
    FILE *file = fopen(HANDLE_NAME, "w+");

    if(!file)
        return 0;
    SLL_Node *curr = ll->SLL->head;
    while(curr)
    {
        char line[100] = "lines/";
        BusLine *DL = ((BusLine*)curr->info);
        if(!DL) continue;
        
        if(snprintf(line, sizeof(line), "lines/%s.dat", DL->name) >= sizeof(line))
            continue;
        
        fprintf(file,"%s,%s\n", DL->name, DL->enterprise);
        saveStops(DL->list, line);
        curr = curr->next;
    }
    fclose(file);
    return 1;
}

int saveStops(DoubleList *dl, char *path){
    if(!dl|| !path)
        return 0;
    
    FILE *file = fopen(path, "wb+");

    if(!file)
        return 0;

    Node *curr = dl->head;
    
    for (int i = 0; i < dl->size; i++)
    {
        fwrite(curr->info, sizeof(DataType), 1, file);
        curr = curr->next;
    }

    fclose(file);
    return 1;
}

int removeLine(char *path){
    if (!path) return 0;

    char line[50];
    if (snprintf(line, sizeof(line), "lines/%s.dat", path) >= sizeof(line))
        return 0;

    return remove(line) == 0;
}