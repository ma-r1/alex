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

int inicializar_ts();

int buscar_ts(char * lexema);

void insertar_ts(char * lexema);

void destruir_ts();

void imprimir_ts();

#endif	// TS_H