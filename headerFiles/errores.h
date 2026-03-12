#ifndef ERRORES_H
#define ERRORES_H

#include "alex.h"

void alex_error_comentario_multilinea(TOKEN *token);
void alex_error_comentario_anidado(TOKEN *token);
void alex_error_string_sin_cerrar(TOKEN *token);
void alex_error_string_eof_escape(TOKEN *token);
void alex_error_caracter(TOKEN *token, char c);

void se_error_lexema_demasiado_grande(char *ini);
void se_error_archivo_no_abierto(char * ruta);
void se_error_archivo_no_cerrado();

void error_asignacion_memoria();

#endif /* ERRORES_H */