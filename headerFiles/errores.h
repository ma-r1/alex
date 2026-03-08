#ifndef ERRORES_H
#define ERRORES_H

#include "alex.h"

/* Errores concretos — imprimen el mensaje y rellenan el token como EOF */
void alex_error_comentario_multilinea(TOKEN *token);
void alex_error_comentario_anidado(TOKEN *token, int nivel);
void alex_error_string_sin_cerrar(TOKEN *token);
void alex_error_string_eof_escape(TOKEN *token);
void alex_error_caracter(TOKEN *token, char c);

#endif /* ERRORES_H */