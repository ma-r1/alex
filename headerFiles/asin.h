#ifndef ASIN_H
#define ASIN_H

/*
  Función principal del analizador sintáctico. 
  Va imprimiendo los componentes léxicos que recibe del analizador sintáctico junto a su correspondiente lexema
  y liberando la memoria reservada para los tokens (y sus lexemas) conforme acaba de usarlos.
  Finaliza cuando recibe el token de EOF.
*/
void imprimir_componentes_lexicos();

#endif    // ALEX_H