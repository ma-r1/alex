#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TMN_BLOQUE 2000

#define CENTINELA EOF

char bufferA[TMN_BLOQUE+1];
char bufferB[TMN_BLOQUE+1];

char *ini, *del;


void _reiniciar_se() {
  ini = bufferA;
  del = bufferA;
  memset(bufferA, 0, sizeof(bufferA));
  memset(bufferB, 0, sizeof(bufferB));
  bufferA[-1] = CENTINELA;
  bufferB[-1] = CENTINELA;
}

int inicializar_se(char * ruta){
  
  _reiniciar_se();

  FILE *f = fopen(ruta, "r");
  if (!f) return -1;

  size_t leidos = fread(bufferA, sizeof(char), TMN_BLOQUE, f);
  fclose(f);

  bufferA[leidos] = CENTINELA; 
  bufferB[leidos] = CENTINELA; 

  ini = del = bufferA;

  printf("SE | Inicializado SE. Leídos %zu caracteres.\n", leidos);
  return 0;
}

char * obtener_lexema(){
  if (del < ini) return 0;

  int n = del - ini;
  char *lexema = (char*)malloc(n*sizeof(char)+1);

  strncpy(lexema, ini, n);
  lexema[n] = '\0';
  
  ini = del;
  //printf("SE | Obtenido lexema: %s\n", lexema);
  return lexema;
}

void destruir_se() {
  _reiniciar_se();
}

char siguiente_caracter(){
  char c = (*del);
  if (c==CENTINELA) return EOF;
  //printf("SE | Enviado caracter: %c\n", c);
  del++;
  return c;
}

void devolver_se(){
  if (del>=bufferA){
    del--;
    //printf("SE | Retrocedido un caracter.\n");
    return;
  }
}




