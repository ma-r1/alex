#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "alex.h"
#include "errores.h"
#include "se.h"
#include "ts.h"
#include "definiciones.h"

/*
  Todos los automatas son funciones privadas.
  Reciben el token creado por siguiente_componente_lexico.
  Algunos toman también el caracter actual.
  Devuelven 0 cuando rellenan el token correctamente y -1 en caso contrario.
*/

int _automata_alphanum(TOKEN *token, char c) {
  int estado = 0;

  while (1) {
    switch (estado) {
      case 0: //Leyendo el cuerpo del id
        if (isalnum(c) || c=='_') {
          estado = 0;
          c = siguiente_caracter();
        }
        else estado = 1; //Se leyó un carácter que no puede formar parte de un ID o keyword, por lo que pasamos al estado de aceptación
        break;
      case 1: //Estado de aceptación
        if (c!=EOF) devolver_se(); //Si se finalizó el archivo hace falta devolver el EOF al sistema de entrada para que genere su propio token después
        token->lexema = obtener_lexema();
        token->comp_lexico = buscar_ts(token->lexema); //Consultamos la ts para averiguar si es un id o una keyword
        if (token->comp_lexico == -1) { //Si no se encontró en la ts es una ID así que se inserta en la ts
          token->comp_lexico = ID;
          insertar_ts(token->lexema);
        }
        return 0;
    }
  }
  return 0;
}

//No necesita estados porque es muy simple, sólo busca el cambio de línea (o EOF)
int _automata_comentario_linea(){ 
  char c = siguiente_caracter();
  while (c != '\n' && c != EOF) {
    descartar_lexema(); //Se van descartando las letras leídas porque no se desea conservar el contenido del comentario
    c = siguiente_caracter();
  }
  descartar_lexema(); //para eliminar el \n o EOF
  return 0;
}

int _automata_comentario_multilinea(TOKEN*token){
  int estado = 0;
  char c = siguiente_caracter();

  while (1) {
    descartar_lexema(); //Se van descartando las letras leídas porque no se desea conservar el contenido del comentario
    switch (estado) {
      case 0: //Buscando el * de comienzo de secuencia de cierre
        if (c=='*') estado = 1;
        else if (c == EOF) { //Se ha acabado el archivo antes de cerrar el comentario
          alex_error_comentario_multilinea(token);
          return -1;
        }
        else estado = 0;
        break;
      case 1: //Se encontró el * que puede ser de comienzo de secuencia de cierre, buscamos la /
        if (c=='/') return 0;
        else if (c == EOF) { //Se ha acabado el archivo antes de cerrar el comentario
          alex_error_comentario_multilinea(token);
          return -1;
        }
        else if(c!='*') estado = 0; //El asterisco encontrado no formaba parte de la secuencia de cierre
        else estado = 1; //Si encontramos más asteriscos puede que formen parte de la secuencia de cierre
        break;
    }
    c = siguiente_caracter();
  }
  return 0;
}

int _automata_comentario_anidado(TOKEN*token){
  int estado = 0;
  int nivel = 1; //Nivel de profundidad del anidamiento (comienza en 1 porque ya leímos un /+)
  char c = siguiente_caracter();

  while (1) {
    descartar_lexema(); //Se van descartando las letras leídas porque no se desea conservar el contenido del comentario
    switch (estado) {
      case 0: //Buscando el + de comienzo de la secuencia de cierre
        if (c == '+') estado = 1;
        else if (c == '/') estado = 2;
        else if (c == EOF) { //Se acabó el archivo sin cerrar el comentario
          alex_error_comentario_anidado(token);
          return -1;
        }
        else estado=0;
        break;
      case 1: //Se encontró un + que puede ser de comienzo de la secuencia de cierre, buscamos la /
        if (c == '/') {
          nivel--;
          if (nivel == 0) return 0; //Si no había nivel de profundidad es que han finalizado todos los comentarios
          estado = 0;
        }
        else if (c == EOF) { //Se acabó el archivo sin cerrar el comentario
          alex_error_comentario_anidado(token);
          return -1;
        }
        else if (c == '+') estado = 1; //Nos mantenemos buscando el /
        else estado = 0;
        break;
      case 2: //Se encontró el /
        if (c == '+') { //Se trataba de una nueva sentencia de apertura de comentario
          nivel++; //Nuevo nivel de profundidad
          estado = 0; //Volvemos a comenzar
        }
        else if (c == EOF) { //Se acabó el archivo sin cerrar el comentario
          alex_error_comentario_anidado(token);
          return -1;
        }
        else if (c == '/') estado = 2; 
        else estado = 0;
        break;
    }
    c = siguiente_caracter();
  }
  return 0;
}

int _automata_string(TOKEN *token){
  int estado = 0;
  char c = siguiente_caracter();

  while(1){
    switch (estado){
      case 0: //Leyendo el contenido del string y buscando el " de cierre
        if(c=='\\') estado = 1; //El siguiente char está escapado
        else if(c=='"'){ //Encontramos el " de cierre, aceptamos
          token->lexema = obtener_lexema();
          token->comp_lexico = STRING;
          return 0; 
        }
        else if (c == EOF) { //Se acabó el archivo sin cerrar el string
          free(obtener_lexema());
          alex_error_string_sin_cerrar(token);
          return -1;
        }
        else estado = 0;
        break;
      case 1: //El caracter c está escapado
        if (c == EOF) { //Se trató de escapar el fin del archivo
          free(obtener_lexema());
          alex_error_string_eof_escape(token);
          return -1;
        }
        estado = 0;
        break;
    }
  c = siguiente_caracter();
  }
}

int _automata_binario(TOKEN * token, char c){
  int estado = 0;

  c = siguiente_caracter();
  while(1){
    switch (estado){
      case 0: //Comprobando que tiene el prefijo correcto
        if (c=='b' || c=='B') estado = 1; //Lo tiene, podemos leer el contenido
        else { //No lo tiene, era un número normal que comenzabba por 0
          if (c!=EOF) devolver_se(); //Retroceder el ultimo char
          return -1;
        }
        break;
      case 1: //Primer dígito binario, debe ser obligatoriamente 0, 1 o _
        if (c=='0' || c=='1' || c=='_') estado = 2;
        else  { //No es un número binario bien formado pero de eso se ocupara el analizador sintáctico, aceptamos
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = ENTERO; //En el léxico de D los binarios son enteros
          return 0;
        }
        break;
      case 2: //Leemos el resto de los dígitos
        if (c=='0' || c=='1' || c=='_') estado = 2;
        else { //Aceptación
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = ENTERO; //En el léxico de D los binarios son enteros
          return 0;
        }
        break;
    }
    c = siguiente_caracter();
  }
}

//estado ini permite a la función principal comenzar el autómata en el estado decimal si el primera caracter leído fue un .
int _automata_numero(TOKEN *token, char c, int estadoini){
  int estado = estadoini;

  while (1) {
    switch (estado) {
      case 0://Parte entera
        if (isdigit(c) || c=='_') estado = 0;
        else if(c=='.') estado = 1; //Se pasa a la parte decimal
        else if(c=='e' || c=='E') estado = 2; //Notación científica
        else{ //Aceptación
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = ENTERO;
          return 0;
        }
        break;
      case 1: //Parte decimal
        if (isdigit(c) || c=='_') estado = 1;
        else if (c=='e' || c=='E') estado = 2; //Notación científica
        else{ //Aceptamos
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
      case 2: //Notación científica, buscamos el símbolo de signo opcional
        if (isdigit(c) || c=='_'  || c=='+' || c=='-') estado = 3;
        else{ //Decimal mal formado, pero lo aceptamos y se ocupará el analizador sintáctico
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
      case 3: //Cuerpo del exponente numérico
        if (isdigit(c) || c=='_' ) estado = 3;
        else{ //Aceptamos
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
    }
    c = siguiente_caracter();
  }
  return 0;
}

TOKEN *siguiente_componente_lexico() {
  TOKEN * token = (TOKEN*)malloc(sizeof(TOKEN));
  if (token == NULL) error_asignacion_memoria();
  
  int c;
  while (1) {
    c=siguiente_caracter();

    if (c == ' ' || c == '\n' || c == '\t' || c=='\0') {
      descartar_lexema(); //Ignoramos los espacios en blanco
      continue;
    }

    if (c==EOF){ //Leído fin de archivo
      token->comp_lexico = EOF;
      token->lexema = NULL;
      return token;
    }

    //IDs y keywords
    if (isalpha(c) || c=='_'){
      int res = _automata_alphanum(token, c);
      if (res == 0) return token;
      continue;
    }

    //Números
    if(isdigit(c)){
      if (c == '0') { //Si empieza por 0 quizá es un binario
        int res = _automata_binario(token, c);
        if (res == 0) return token;
      }
      //Si no era binario lo procesamos como un número normal
      int res = _automata_numero(token, c, 0);
      if (res == 0) return token;
      continue;
    }

    if(c=='.'){
      c=siguiente_caracter();
      //Si el . va seguido de un número se trata de un decimal
      if (isdigit(c)){
        int res = _automata_numero(token, c, 1); //Empieza en el estado 1 porque ya ha leído el .
        if (res == 0) return token;
      }
      //Si no, era un punto normal
      if (c!=EOF) devolver_se(); //Devolvemos el caracter que leímos para comprobar (si es EOF)
      token->comp_lexico = (int)'.';
      token->lexema = obtener_lexema();
      return token;
    }

    if (c=='/'){
      c=siguiente_caracter(); 
      //Si el / va seguido de otro / es un comentario de línea
      if (c=='/'){
        _automata_comentario_linea();
        continue;
      }
      //Si va seguido de un * es un comentario multilínea
      else if (c=='*'){
        int res = _automata_comentario_multilinea(token);
        if (res==-1) return token;
        continue;
      }
      //Si va seguido de un + es un comentario anidado
      else if (c=='+'){
        int res = _automata_comentario_anidado(token);
        if (res==-1) return token;
        continue;
      }
      //Si no, es simplemente el operador /
      if (c!=EOF) devolver_se(); //Devolvemos el caracter que leímos para comprobar
      token->comp_lexico = (int)'/';
      token->lexema = obtener_lexema();
      return token;
    }

    if(c=='='){
      c=siguiente_caracter();
      //Si el siguiente caracter es =, es el operador ==
      if(c=='=') token->comp_lexico = DOBLEIGUAL;
      //Si no, es el operador =
      else {
        if (c!=EOF) devolver_se(); //Devolvemos el caracter que leímos para comprobar
        token->comp_lexico = (int)'=';
      }
      token->lexema = obtener_lexema();
      return token;
    }

    if(c=='+'){
      c=siguiente_caracter();
      //Si el siguiente caracter es =, es el operador +=
      if(c=='=') token->comp_lexico = MASIGUAL;
      //Si el siguiente caracter es +, es el operador ++
      else if(c=='+') token->comp_lexico = MASMAS;
      //Si no, es el operador +
      else {
        if (c!=EOF) devolver_se(); //Devolvemos el caracter que leímos para comprobar
        token->comp_lexico = (int)'+';
      }
      token->lexema = obtener_lexema();
      return token;
    }
    
    if(c=='"'){
      _automata_string(token);
      return token;
    }

    //Operadores de un caracter
    switch (c) {
      case ';': case '(': case ')': case '[':
      case ']': case '{': case '}': case ',':
      case '-': case '*': case '<':
        token->comp_lexico = (int)c; //El código del componente léxico es simplemente el ASCII del operador
        token->lexema = obtener_lexema();
        return token;
    }

    //Si llegamos aquí es que encontramos un caracter desconocido
    free(obtener_lexema()); //Limpiar el buffer
    alex_error_caracter(token, (char)c);
    return token;
  }
}