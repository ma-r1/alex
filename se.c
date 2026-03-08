#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errores.h"
#include "se.h"

#define CENTINELA EOF

typedef struct {
  char buffer[TMN_MAX_LEXEMA+1];
  char * inicio;
  char * fin;
} BLOQUE;

BLOQUE bloqueA, bloqueB;

char *ini, *del;

FILE * archivo;

char * lexema = NULL;

void _del_masmas(){
  if (del==bloqueA.fin) del=bloqueB.inicio;
  else if (del==bloqueB.fin) del=bloqueA.inicio;
  else del++;
}

void _del_menosmenos(){
  if (del==bloqueA.inicio) del=bloqueB.fin-1;
  else if (del==bloqueB.inicio) del=bloqueA.fin-1;
  else del--;
}

int _puntero_pertenece_a_bloque(char * puntero, BLOQUE bloque){
  return(puntero>=bloque.inicio && puntero<=bloque.fin);
}

int _cargar_bloque(BLOQUE  *bloque){
  if(_puntero_pertenece_a_bloque(ini, *bloque) && ini!=del){ //estoy pisando un bloque no analizado si ini está en él pero del no coincide con ini (si coinciden es porque acabamos de reiniciar)
    se_error_lexema_demasiado_grande(ini);
    ini=del;
  }
  int leidos = fread(bloque->buffer, sizeof(char), TMN_MAX_LEXEMA, archivo);
  if (leidos==0){
    se_error_archivo_no_acceso();
  }
  else if (leidos < TMN_MAX_LEXEMA) {
    bloque->buffer[leidos] = EOF;
  }
  //printf("SE | Cargado bloque %s.\n", bloque->buffer == bloqueA.buffer ? "bloqueA" : "bloqueB");
  return leidos;
}


void _reiniciar_se() {
  bloqueA.buffer[TMN_MAX_LEXEMA] = CENTINELA;
  bloqueB.buffer[TMN_MAX_LEXEMA] = CENTINELA;

  bloqueA.inicio=bloqueA.buffer;
  bloqueB.inicio=bloqueB.buffer;
  bloqueA.fin=&(bloqueA.buffer[TMN_MAX_LEXEMA]);
  bloqueB.fin=&(bloqueB.buffer[TMN_MAX_LEXEMA]);

  ini = bloqueA.buffer;
  del = bloqueA.buffer;
}



int inicializar_se(char * ruta){
  _reiniciar_se();
  archivo = fopen(ruta, "r");
  if (archivo == NULL) se_error_archivo_no_abierto(ruta);
  _cargar_bloque(&bloqueA);

  printf("SE | Inicializado SE.\n");
  return 0;
}

char * obtener_lexema(){
  int n = 0;

  if(_puntero_pertenece_a_bloque(ini, bloqueA) && _puntero_pertenece_a_bloque(del, bloqueB)){
    n = (bloqueA.fin - ini) + (del - bloqueB.inicio);
  }
  else if (_puntero_pertenece_a_bloque(ini, bloqueB) && _puntero_pertenece_a_bloque(del, bloqueA)){
    n = (bloqueB.fin - ini) + (del - bloqueA.inicio);
  }
  else n = del - ini;

  lexema = (char*)malloc(n*sizeof(char)+1);
  if (lexema == NULL) error_asignacion_memoria();

  if(_puntero_pertenece_a_bloque(ini, bloqueA) && _puntero_pertenece_a_bloque(del, bloqueB)){
    strncpy(lexema, ini, bloqueA.fin - ini);
    strncpy(lexema + (bloqueA.fin - ini), bloqueB.inicio, del - bloqueB.inicio);
  }
  else if (_puntero_pertenece_a_bloque(ini, bloqueB) && (_puntero_pertenece_a_bloque(del, bloqueA))){
    strncpy(lexema, ini, bloqueB.fin - ini);
    strncpy(lexema + (bloqueB.fin - ini), bloqueA.inicio, del - bloqueA.inicio);
  }
  else {
    strncpy(lexema, ini, n);
  }
  lexema[n]='\0';
  ini=del;
  //printf("SE | Obtenido lexema %s.\n", lexema);
  return lexema;
}

void destruir_se() {
  _reiniciar_se();
  if (EOF==fclose(archivo)) se_error_archivo_no_acceso();
}

char siguiente_caracter(){
  char c = (*del);
  if (c == CENTINELA) {
    if (del == bloqueA.fin) {
      _cargar_bloque(&bloqueB);
      del = bloqueB.inicio;
      c = *del;
    } 
    else if (del == bloqueB.fin) {
      _cargar_bloque(&bloqueA);
      del = bloqueA.inicio;
      c = *del; 
    }
    if (c == EOF) return EOF;
  }
  _del_masmas();
  //printf("SE | Enviado caracter %c.\n", c);
  return c;
}

void devolver_se(){
  //printf("SE | Retrocedido caracter.\n");
  _del_menosmenos();
  return;
}




