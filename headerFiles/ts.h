/*
Fundamentalmente ha de contener dos funciones imprescindibles: 
1) una función de búsqueda que permita recorrer la tabla de símbolos para no 
replicar innecesariamente el mismo componente léxico; 
2) una función de inserción que permita almacenar un nuevo 
componente léxico encontrado.
*/

#ifndef TS_H
#define TS_H

#include "abb.h"

typedef TABB TS;

/*
  Inicializa el ABB e inserta las keywords.
*/

void inicializar_ts();

/*
  Busca un lexema concreto en la tabla de símbolos.
  Devuelve el componente léxico de dicho lexema si lo encuentra o -1 en caso contrario.
*/
int buscar_ts(char * lexema);

/*
  Inserta un nuevo identificador en la tabla de símbolos con el lexema especificado.
*/
void insertar_ts(char * lexema);

/*
  Libera la memoria reservada para el abb.
*/
void destruir_ts();

/*
  Recorre y muestra por consola todo el contenido de la tabla de símbolos
*/
void imprimir_ts();

#endif	// TS_H