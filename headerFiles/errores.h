#ifndef ERRORES_H
#define ERRORES_H

#include "alex.h"

/*
  Las siguientes funciones imprimen un error por pantalla indicando la línea del código fuente donde occure.
  Modifican el token que se les pasa por referencia para convertirlo en uno de EOF que detenga el analizador sintáctico.
*/

void alex_error_comentario_multilinea(TOKEN *token);
void alex_error_comentario_anidado(TOKEN *token);
void alex_error_string_sin_cerrar(TOKEN *token);
void alex_error_string_eof_escape(TOKEN *token);
void alex_error_caracter(TOKEN *token, char c); //c es el caracter no reconocido que provoca el error

/*
  Avisa de que el lexema que se trata de leer es demasiado largo y se está intentando sobreescribir a sí mismo en el doble buffer.
  Como solución, el programa trunca este lexema y crea un token con el componente léxico adecuado y el final del lexema correspondiente para continuar leyendo.
  Toma como argumento
*/
void se_error_lexema_demasiado_grande();

/*
  Estos errores son fatales y finalizan la ejecución del programa (tras liberar la memoria reservada hasta el momento).
*/
void se_error_archivo_no_abierto(char * ruta); //Toma como argumento la ruta del archivo que no se consiguió abrir
void se_error_archivo_no_cerrado();
void error_asignacion_memoria();

#endif /* ERRORES_H */