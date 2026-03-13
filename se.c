#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errores.h"
#include "se.h"

#define CENTINELA EOF

//Estructura para cada uno de los buffers
typedef struct {
  char buffer[TMN_MAX_LEXEMA+1]; //El buffer en sí
  char * inicio; //Puntero al primer elemento del buffer
  char * fin; //Puntero al último elemento del buffer
} BLOQUE;

BLOQUE bloqueA, bloqueB; //Los dos buffers

char *ini, *del; //ini apunta al inicio del lexema que se está analizando en el momento, del al final actual del lexema

FILE * archivo;

char * lexema = NULL; //lexema que se está leyendo

int num_linea = 1;

int error_impreso = 0; //bandera para evitar que se imprima varias veces el error de lexema demasiado largo

/*
  Función privada.
  Avanza el puntero del una posición.
*/
void _del_masmas(){
  if (del==bloqueA.fin) del=bloqueB.inicio;
  else if (del==bloqueB.fin) del=bloqueA.inicio;
  else del++;
}

/*
  Función privada.
  Retrocede el puntero del una posición.
*/
void _del_menosmenos(){
  if (del==bloqueA.inicio) del=bloqueB.fin-1; //No se coloca en la última posición del bloque porque ahí está el ceninela
  else if (del==bloqueB.inicio) del=bloqueA.fin-1;
  else del--;
}

/*
  Función privada.
  Comprueba si el puntero está apuntando a una de las posiciones del buffer del bloque.
*/
int _puntero_pertenece_a_bloque(char * puntero, BLOQUE bloque){
  return(puntero>=bloque.inicio && puntero<=bloque.fin);
}

/*
  Función privada.
  Lee el archivo fuente para cargar uno de los buffers.
*/
void _cargar_bloque(BLOQUE  *bloque){
  //Si tratar de cargar un bloque vemos que el puntero ini está dentro de él, estamos pisando el comienzo del lexema que aún estamos analizando

  if(_puntero_pertenece_a_bloque(ini, *bloque) && ini!=del){ 
    if (!error_impreso) { //Para evitar que el error se imprima varias veces en lexemas muy grandes
      se_error_lexema_demasiado_grande(ini); //Imprime mensaje de error
      error_impreso=1; //Establece flag
    }
    del = bloque->inicio; //Movemos del al comienzo del bloque que acabamos de cargar
    ini = del; //Movemos ini a la posición de del para descartar el comienzo del lexema
  }
  int leidos = fread(bloque->buffer, sizeof(char), TMN_MAX_LEXEMA, archivo); //Leer el archivo fuente
  if (leidos < TMN_MAX_LEXEMA) { //Si ya estamos acabando el archivo y no quedan caracteres suficientes para rellenar el buffer, colocamos el EOF justo después del último caracter leído
    bloque->buffer[leidos] = EOF;
  }
  return;
}

/*
  Función privada.
  Reinicia el contenido de los buffers, coloca los centinelas y resetea los punteros.
*/
void _reiniciar_se() {
  memset(bloqueA.buffer, 0, TMN_MAX_LEXEMA);
  memset(bloqueB.buffer, 0, TMN_MAX_LEXEMA);

  bloqueA.buffer[TMN_MAX_LEXEMA] = CENTINELA;
  bloqueB.buffer[TMN_MAX_LEXEMA] = CENTINELA;

  bloqueA.inicio=bloqueA.buffer;
  bloqueB.inicio=bloqueB.buffer;
  bloqueA.fin=&(bloqueA.buffer[TMN_MAX_LEXEMA]);
  bloqueB.fin=&(bloqueB.buffer[TMN_MAX_LEXEMA]);

  ini = bloqueA.buffer;
  del = bloqueA.buffer;

  num_linea=1;
}

void descartar_lexema() { 
  //Cuando no tenemos interés de conservar el lexema (comentarios, espacios, tabulaciones, etc.) avanzamos inicio y delantero de uno en uno sin extraer nada
  ini = del;
  error_impreso = 0;
}

void inicializar_se(char * ruta){
  _reiniciar_se();

  archivo = fopen(ruta, "r"); //Abrir el archivo fuente
  if (archivo == NULL) se_error_archivo_no_abierto(ruta);
  _cargar_bloque(&bloqueA); //Cargar el primer bloque
  //No cargamos un segundo bloque, porque si no al acabar de leer el primero pisaríamos el segundo
  return;
}

char * obtener_lexema(){
  error_impreso = 0; //Extraímos un lexema por lo que bajamos la bandera del error

  int n = 0; //Tamaño del lexema

  //Si el lexema comienza en el bloque A y finaliza en el B
  if(_puntero_pertenece_a_bloque(ini, bloqueA) && _puntero_pertenece_a_bloque(del, bloqueB)){
    n = (bloqueA.fin - ini) + (del - bloqueB.inicio);
  }
  //Si el lexema comienza en el bloque B y finaliza en el A
  else if (_puntero_pertenece_a_bloque(ini, bloqueB) && _puntero_pertenece_a_bloque(del, bloqueA)){
    n = (bloqueB.fin - ini) + (del - bloqueA.inicio);
  }
  //Si el lexema está contenido en un único bloque 
  else n = del - ini;

  lexema = (char*)malloc(n*sizeof(char)+1); //+1 para añadir el \0
  if (lexema == NULL) error_asignacion_memoria();

  //Si el lexema comienza en el bloque A y finaliza en el B
  if(_puntero_pertenece_a_bloque(ini, bloqueA) && _puntero_pertenece_a_bloque(del, bloqueB)){
    strncpy(lexema, ini, bloqueA.fin - ini); //Primera parte
    strncpy(lexema + (bloqueA.fin - ini), bloqueB.inicio, del - bloqueB.inicio); //Segunda parte
  }
  //Si el lexema comienza en el bloque B y finaliza en el A
  else if (_puntero_pertenece_a_bloque(ini, bloqueB) && (_puntero_pertenece_a_bloque(del, bloqueA))){
    strncpy(lexema, ini, bloqueB.fin - ini); //Primera parte
    strncpy(lexema + (bloqueB.fin - ini), bloqueA.inicio, del - bloqueA.inicio); //Segunda parte
  }
  //Si el lexema está contenido en un único bloque 
  else {
    strncpy(lexema, ini, n);
  }
  lexema[n]='\0'; //Añadimos el fin de cadena
  ini=del; //Avanzamos ini para prepararnos para analizar otro lexema
  return lexema;
}

void destruir_se() {
  _reiniciar_se();
  if (archivo != NULL) { if (EOF==fclose(archivo)) se_error_archivo_no_cerrado(); };
}

char siguiente_caracter(){
  char c = (*del);
  if (c == CENTINELA) {
    //Fin de bloque A
    if (del == bloqueA.fin) {
      _cargar_bloque(&bloqueB);
      del = bloqueB.inicio;
      c = *del;
    } 
    //Fin de bloque B
    else if (del == bloqueB.fin) {
      _cargar_bloque(&bloqueA);
      del = bloqueA.inicio;
      c = *del; 
    }
    //Fin de archivo
    if (c == EOF) return EOF;
  }
  _del_masmas(); //Avanzar delantero

  if (c=='\n') num_linea++;
  return c;
}

void devolver_se(){
  _del_menosmenos();
  if (*del=='\n') num_linea--;
  return;
}

int obtener_linea() { return num_linea; }