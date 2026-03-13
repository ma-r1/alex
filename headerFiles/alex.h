#ifndef ALEX_H
#define ALEX_H

#include "token.h"

/* 
  Función principal del analizador léxico.
  Devuelve un puntero a un nuevo token con un componente léxico y un lexema.
  Si llega al final del archivo devuelve un token con el componente léxico EOF para que el analizador sintático termine.
*/
TOKEN *siguiente_componente_lexico();

#endif    // ALEX_H