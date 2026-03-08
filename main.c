#include <stdio.h>
#include <stdlib.h>

#include "alex.h"
#include "asin.h"
#include "se.h"
#include "ts.h"

int main() {
  /*TS ts;

  if (inicializar_ts(&ts) == -1) {
    fprintf(stderr, "Error al inicializar la tabla de símbolos\n");
    return EXIT_FAILURE;
  }

  if (inicializar_se(filename) != 0) {
    fprintf(stderr, "Error al inicializar el analizador léxico\n");
    destruir_ts(&ts);
    return EXIT_FAILURE;
  }

  analisis_sin();

  destruir_se();
  destruir_ts(&ts);
  return 0;*/
  inicializar_ts();
  imprimir_ts();
  inicializar_se("./regression.d");
  imprimir_componentes_lexicos();
  imprimir_ts();
  return 0;
}


