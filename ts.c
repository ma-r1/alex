#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ts.h"
#include "token.h"
#include "definiciones.h"
#include "abb.h"

static TS ts; //Almacena el abb

void inicializar_ts() {
  crearAbb(&ts);

  //Diccionario con las keywords del lenguaje D
  TOKEN keywords[] = {
      {IMPORT, "import"},
      {DOUBLE, "double"},
      {INT, "int"},
      {FOREACH, "foreach"},
      {RETURN, "return"},
      {VOID, "void"},
      {CAST, "cast"},
      {WHILE, "while"}
  };

  int n = sizeof(keywords) / sizeof(TOKEN);

  //Insertar todas las keywords en el árbol
  for (int i = 0; i < n; i++) 
      insertarElementoAbb(&ts, keywords[i]);

  return;
}

int buscar_ts(char * lexema){
  TOKEN token;
  token.comp_lexico = 0;
  buscarNodoAbb(ts, lexema, &token);
  if (token.comp_lexico != 0) return token.comp_lexico; //si se modificó el valor del componente léxico es que lo encontró en el abb, así que lo devolvemos
  return -1; //en caso contrario no se encontró en el abb
}

void insertar_ts(char * lexema){
  TOKEN token;
  token.comp_lexico = ID;
  token.lexema = lexema;
  insertarElementoAbb(&ts, token);
}

void destruir_ts(){
  destruirAbb(&ts);
}

/*
  Función privada.
  Permite imprimir el abb sin tener que poner la TS como argumento de la función pública (así la variable de la ts puede ser privada a este archivo)
*/
void _imprimir_recursivo(TS a){
  if(!esAbbVacio(a)){
    TOKEN token;

    _imprimir_recursivo(izqAbb(a));

    leerElementoAbb(a, &token);
    printf("|  %d  |  %s\n", token.comp_lexico, token.lexema);
    
    _imprimir_recursivo(derAbb(a));
  }
}

void imprimir_ts(){
  _imprimir_recursivo(ts);
}