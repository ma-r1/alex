#include <stdio.h>
#include <stdlib.h>

#include "alex.h"
#include "asin.h"
#include "se.h"
#include "ts.h"

int main(int argc, char * argv[]) {
  inicializar_ts();
  imprimir_ts();
  int estado_se;
  if (argc<2) estado_se=inicializar_se("./regression.d");
  else estado_se=inicializar_se(argv[1]);
  if (estado_se == -1){
    //TODO error
  }
  imprimir_componentes_lexicos();
  imprimir_ts();
  return 0;
}


