/*
Puede contener las definiciones de los componentes léxicos del lenguaje D 
utilizados en regresssion.d. Es suficiente con identificar cada componente 
léxico con un número entero, que el analizador léxico devolverá cada vez 
que sea invocado. Además, podemos definir aquí la tabla de símbolos como 
un vector de estructuras que permite almacenar cada uno de los componentes 
léxicos, y el lexema correspondiente en aquellos casos en los que sea necesario.
*/
#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#define ID 301
#define STRING 312
#define ENTERO 313
#define DECIMAL 314

//keywords
#define IMPORT 300
#define DOUBLE 302
#define INT 303
#define FOREACH 304
#define RETURN 305
#define VOID 306
#define CAST 307
#define WHILE 308

//simbolos
#define DOBLEIGUAL 309
#define MASIGUAL 310
#define MASMAS 311

#endif //DEFINICIONES_H