/*
Desde aquí se accede al fichero regression.d para analizarlo 
como un flujo de caracteres. El sistema de entrada ha de devolver 
el siguiente carácter del código fuente cada vez que es invocado, 
y ha de permitir devolver caracteres al flujo de entrada si es necesario. 
Podemos, por tanto, considerar para ello el diseño de dos funciones.
*/
#ifndef SE_H
#define SE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TMN_MAX_LEXEMA 5

void descartar_lexema();

int inicializar_se(char * ruta);

char * obtener_lexema();

void destruir_se();

char siguiente_caracter();

void devolver_se();

int obtener_linea();

#endif	// SE_H

