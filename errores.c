#include <stdio.h>
#include <stdlib.h>

#include "errores.h"
#include "se.h"
#include "ts.h"

/*
  Función privada.
  Convierte el token actual en un EOF para que se detenga el analizador sintáctico.
*/
void _alex_eof_token(TOKEN *token) {
  token->comp_lexico = EOF;
  token->lexema = NULL;
}

void alex_error_comentario_multilinea(TOKEN *token) {
  fprintf(stderr, "Línea %d: comentario multilínea sin cerrar.\n", obtener_linea());
  _alex_eof_token(token);
}

void alex_error_comentario_anidado(TOKEN *token) {
  fprintf(stderr, "Línea %d comentario anidado sin cerrar.\n", obtener_linea());
  _alex_eof_token(token);
}

void alex_error_string_sin_cerrar(TOKEN *token) {
  fprintf(stderr, "Línea %d: string sin cerrar.\n", obtener_linea());
  _alex_eof_token(token);
}

void alex_error_string_eof_escape(TOKEN *token) {
  fprintf(stderr, "Línea %d: EOF tras secuencia de escape en string.\n", obtener_linea());
  _alex_eof_token(token);
}

void alex_error_caracter(TOKEN *token, char c) {
  fprintf(stderr, "Línea %d: carácter no reconocido '%c'.\n", obtener_linea(), c);
  _alex_eof_token(token);
}

void se_error_lexema_demasiado_grande(){ //cuando sucede esto el token creado contiene un lexema probablemente incompleto o vacío, pero con el componente léxico que le corresponde para que el analizador sintáctico pueda continuar buscando errores después
  fprintf(stderr, "Línea %d: lexema demasiado largo.\n", obtener_linea());
}

void se_error_archivo_no_abierto(char * ruta){
  fprintf(stderr, "Error sistema de entrada: no se pudo abrir el archivo '%s'.\n", ruta);
  destruir_ts(); //Destruimos la tabla de símbolos que inicializó el main
  exit(1); //Es un error grave, cerramos
}

void se_error_archivo_no_cerrado(){
  fprintf(stderr, "Error sistema de entrada: no se pudo cerrar el archivo.\n");
  destruir_ts();
  exit(1); //Es un error grave, cerramos
}

void error_asignacion_memoria(){
  fprintf(stderr, "Error crítico: no se pudo asignar memoria.\n");
  destruir_ts(); //Destruimos la ts que inicializó el main
  destruir_se(); //Destruimos el sistema de entrada que inicializó el main
  exit(1); //Es un error grave, cerramos
}
