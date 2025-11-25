#include <search.h>
#include <wchar.h>

int search_to_array(Object *information, wchar_t ***array, int *array_length,
                    wchar_t *string) {
  SLL_Node *node;
  *array_length = 0;
  *array = NULL;

  for (node = information->SLL->head; node != NULL; node = node->next) {
    Node *inicial = (((BusLine *)(node->info))->list->head);
    Node *current = inicial;
    if (current != NULL) {
      do {
        if (like((wchar_t *)(current->info->nome), string)) {
          wchar_t **tmp =
              realloc(*array, sizeof(wchar_t *) * (*array_length + 1));

          if (!tmp)
            return 0;

          *array = tmp;

          (*array)[*array_length] = (wchar_t *)current->info->nome;

          (*array_length)++;
        }
        current = current->next;
      } while (current != inicial);
    }
  }

  return (*array_length > 0);
}

int like(wchar_t *search, wchar_t *string) {
  if (!search || !string)
    return 0;

  size_t len = wcslen(search);

  for (const wchar_t *p = string; *p != L'\0'; p++) {
    if (wcsncmp(p, search, len) == 0)
      return 1;
  }
  return 0;
}

wchar_t *search_route(Object *information, wchar_t *first_stop,
                      wchar_t *second_stop, Hours departure_time,
                      Hours arrival_time) {
  SLL_Node *fNode;

  for (fNode = information->SLL->head; fNode != NULL; fNode = fNode->next) {
    Node *inicial = (((BusLine *)(fNode->info))->list->head);
    Node *arrNode = inicial;
    if (arrNode != NULL) {
      do {
        if (like((wchar_t *)(arrNode->info->nome), second_stop)) {
          Node *deparNode = arrNode;
          for (deparNode = deparNode->prev; deparNode != arrNode;
               deparNode = deparNode->prev) {
            if (like((wchar_t *)(deparNode->info->nome), first_stop) &&
                deparNode->info->departure_time.hours == departure_time.hours &&
                deparNode->info->departure_time.minutes ==
                    departure_time.minutes) {
              wchar_t *resultado = malloc(sizeof(wchar_t) * 512);
              swprintf(resultado, 512,
                       L"%ls para %ls\n\nHorario de saída: %d:%d\nHorario de "
                       L"chegada: %d:%d\n\nLinha: %d\nNome da companhia: %ls",
                       first_stop, second_stop,
                       deparNode->info->departure_time.hours,
                       deparNode->info->departure_time.minutes,
                       deparNode->info->arrival_time.hours,
                       deparNode->info->arrival_time.minutes,
                       ((BusLine *)fNode->info)->enterprise_ID,
                       ((BusLine *)fNode->info)->enterprise);

              return resultado;
            }
          }
        }
        arrNode = arrNode->next;
      } while (arrNode != inicial);
    }
  }
  return L"Não foi possível encontrar";
}
