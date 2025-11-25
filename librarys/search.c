#include <search.h>
#include <wchar.h>

int search_to_array(Object *information, wchar_t ***array, int *array_length,
                    wchar_t *string) {
  SLL_Node *node;
  *array_length = 0;

  for (node = information->SLL->head; node != NULL; node = node->next) {
    if (like((wchar_t *)node->info, string)) {
      wcscpy(
          *array[*array_length],
          (wchar_t *)node->info); // usar o array_lenth para acessar a posicao
                                  // livre do array (Genial eu sei heheheheh)
      *array_length++;
    }
  }

  return (*array_length > 0);
}

int like(wchar_t *search, wchar_t *string) {
  for (wchar_t *srch = search, *str = string; str!=NULL; str++) {
        if(*srch == *str){ // procurar em string algum carectere que seja igual ao primeiro caracter de search
            for( ;srch!=NULL;srch++,str++){ // quando achado faz um for a partir dele caracter
                if(str==NULL) // caso string chegue ao fim retorna 0
                    return 0;
                if(*srch!=*str) // caso string eh diferente de search retorna 0
                    return 0;
            }
            return 1; // retornara 1 caso search chegou ao fim e todos os caracteres foram iguais ao q tem em string
        }
  }
}

wchar_t *search_route(Object *information, wchar_t *first_stop,
                      wchar_t *second_stop, Hours departure_time,
                      Hours arrival_time);
