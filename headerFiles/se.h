#ifndef SE_H
#define SE_H

#define TMN_MAX_LEXEMA 256

/*
  Inicializa el sistema de entrada inicializando el sistema de doble buffer, abriendo el archivo y cargando el primer bloque.
  El argumento ruta debe ser un string a la ruta del archivo fuente.
*/
void inicializar_se(char * ruta);

/*
  Devuelve el caracter actual apuntado por delantero (EOF si se finalizó el archivo) y avanza dicho puntero.
*/
char siguiente_caracter();

/*
  Retrocede el delantero una posición.
*/
void devolver_se();

/*
  Extrae el lexema contenido entre los punteros inicio y delantero y devuelve un puntero a este lexema.
*/
char * obtener_lexema();

/*
  Iguala el puntero inicio al de delantero para vaciar el lexema leído hasta el momento sin extraerlo.
  Se usa para ignorar los comentarios, espacios, tabulaciones y saltos de línea.
*/
void descartar_lexema();

/*
  Cierra el archivo fuente.
*/
void destruir_se();

/*
  Función auxiliar para el sistema de errores que devuelve el número de línea que se está analizando.
*/
int obtener_linea();

#endif	// SE_H

