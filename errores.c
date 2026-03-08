#include <stdio.h>

#include "errores.h"
#include "alex.h"
#include "definiciones.h"

void _alex_eof_token(TOKEN *token) {
  token->comp_lexico = EOF;
  token->lexema = NULL;
}

void alex_error_comentario_multilinea(TOKEN *token) {
  fprintf(stderr, "AL | Error léxico: comentario multilínea '/* ... */' sin cerrar.\n");
  _alex_eof_token(token);
}

void alex_error_comentario_anidado(TOKEN *token, int nivel) {
  fprintf(stderr, "AL | Error léxico: comentario anidado '/+ ... +/' sin cerrar (nivel=%d).\n", nivel);
  _alex_eof_token(token);
}

void alex_error_string_sin_cerrar(TOKEN *token) {
  fprintf(stderr, "AL | Error léxico: string sin cerrar.\n");
  _alex_eof_token(token);
}

void alex_error_string_eof_escape(TOKEN *token) {
  fprintf(stderr, "AL | Error léxico: EOF tras secuencia de escape en string.\n");
  _alex_eof_token(token);
}

void alex_error_caracter(TOKEN *token, char c) {
  fprintf(stderr, "AL | Error léxico: carácter no reconocido '%c'.\n", c);
  _alex_eof_token(token);
}