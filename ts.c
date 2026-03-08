#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ts.h"
#include "token.h"
#include "definiciones.h"
#include "abb.h"

static TS ts;

int inicializar_ts() {
  crearAbb(&ts);

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

  for (int i = 0; i < n; i++) 
      insertarElementoAbb(&ts, keywords[i]);

  printf("TS | Tabla de símbolos inicializada con %d palabras reservadas.\n", n);
  return 1;
}

int buscar_ts(char * lexema){
  TOKEN token;
  token.comp_lexico = 0;
  buscarNodoAbb(ts, lexema, &token);
  if (token.comp_lexico != 0){
    //printf("TS | Encontrado %s en la TS.\n", lexema);
    return token.comp_lexico;
  }
  return -1;
}

void insertar_ts(char * lexema){
  TOKEN token;
  token.comp_lexico = ID;
  token.lexema = lexema;
  insertarElementoAbb(&ts, token);
  //printf("TS | Insertado %s en la TS.\n", lexema);
}

void destruir_ts(){
  destruirAbb(&ts);
}

void _imprimir_recursivo(TS a){ //necesario para imprimir sin necesidad de pasar la TS por argumento a la función pública
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