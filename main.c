#include <stdio.h>
#include <stdlib.h>

#include "asin.h"
#include "se.h"
#include "ts.h"

int main(int argc, char * argv[]) {
  inicializar_ts();

  imprimir_ts();

  //Si no se especifica un archivo fuente se usará regression.d (suponiendo que se encuentra en el directorio actual)
  if (argc<2) inicializar_se("./regression.d");
  else inicializar_se(argv[1]);

  imprimir_componentes_lexicos();

  imprimir_ts();

  destruir_se();

  destruir_ts();

  return 0;
}


