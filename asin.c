#include <stdio.h>
#include <stdlib.h>

#include "alex.h"

void imprimir_componentes_lexicos() {
  while (1) {
    TOKEN *token = siguiente_componente_lexico();
    if (!token) {
      printf("AS | No se ha recibido token.\n");
      //TODO error
      break;
    }
    if (token->comp_lexico == EOF){
      printf("AS | EOF encontrado, fin anális.\n");
      free(token->lexema);
      free(token);
      break;
    }
    printf("<%d, %s>\n", token->comp_lexico, token->lexema);
    free(token->lexema);
    free(token);
  }
}
