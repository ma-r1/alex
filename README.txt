- - - INFORMACIÓN
* Versión de GCC usada: 13.3.0
* Versión de Ubuntu usada: Ubuntu 24.04.4 LTS

- - - INSTRUCCIONES DE COMPILACIÓN
Para la compilación del código fuente se usa el makefile. 
* Para crear el ejecutable se usa la regla predeterminada:
  $> make
* Para eliminar los ficheros .o se usa la regla clean:
  $> make clean
* Para eliminar los ficheros .o y el ejecutable se usa la regla cleanall:
  $> make cleanall

- - - INSTRUCCIONES DE EJECUCIÓN
El ejecutable creado con el make file se llama 'main'.
* Tiene un único argumento de ejecución, que es la ruta del archivo con el código fuente a analizar.
  $> ./main ./codigo_fuente.d
* Si no se especifica este argumento, se analizará el archivo de ruta './regression.d'.
  $>./main
