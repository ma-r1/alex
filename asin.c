#include <stdio.h>
#include <stdlib.h>

#include "alex.h"

void imprimir_componentes_lexicos() {
  //se van pidiendo tokens hasta encontrar el EOF
  while (1) {
    TOKEN *token = siguiente_componente_lexico();
    if (token->comp_lexico == EOF){
      //el token de EOF no se imprime
      free(token); //liberamos la memoria reservada para el token
      break; //salimos al encontrar EOF
    }
    printf("<%d, %s>\n", token->comp_lexico, token->lexema);
    free(token->lexema); //liberamos la memoria reservada para el lexema
    free(token); //liberamos la memoria reservada para el toekn
  }
}
