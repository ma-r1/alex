#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "alex.h"
#include "errores.h"
#include "se.h"
#include "ts.h"
#include "definiciones.h"

/*Los autómatas devuelven 0 cuando rellenan el token correctamente 
y -1 cuando encuentran un error léxico*/

int _automata_alphanum(TOKEN *token, char c) {
  int estado = 0;

  while (1) {
    switch (estado) {
      case 0:
        if (isalnum(c) || c=='_') {
          estado = 0;
          c = siguiente_caracter();
        }
        else estado = 1;
        break;
      case 1:
        if (c!=EOF) devolver_se();
        token->lexema = obtener_lexema();
        token->comp_lexico = buscar_ts(token->lexema);
        if (token->comp_lexico == -1) { //si no se encontró en la TS
          token->comp_lexico = ID;
          insertar_ts(token->lexema);
        }
        return 0;
    }
  }
  return 0;
}

int _automata_comentario_linea(){ //no necesita estados porque sólo busca el cambio de línea (o EOF)
  char c = siguiente_caracter();
  while (c != '\n' && c != EOF) {
    c = siguiente_caracter();
  }
  return 0;
}

int _automata_comentario_multilinea(TOKEN*token){
  int estado = 0;
  char c = siguiente_caracter();

  while (1) {
    switch (estado) {
      case 0:
        if (c=='*') estado = 1;
        else if (c == EOF) {
          free(obtener_lexema());
          alex_error_comentario_multilinea(token);
          return -1;
        }
        else estado = 0;
        break;
      case 1:
        if (c=='/') estado = 2;
        else if (c == EOF) {
          free(obtener_lexema());
          alex_error_comentario_multilinea(token);
          return -1;
        }
        else if(c!='*') estado = 0;
        else estado = 1;
        break;
      case 2:
        free(obtener_lexema());
        return 0;
    }
    c = siguiente_caracter();
  }
  return 0;
}

int _automata_comentario_anidado(TOKEN*token){
  int estado = 0;
  int nivel = 1;
  char c = siguiente_caracter();

  while (1) {
    switch (estado) {
      case 0:
        if (c == '+') estado = 1;
        else if (c == '/') estado = 2;
        else if (c == EOF) {
          free(obtener_lexema());
          alex_error_comentario_anidado(token, nivel);
          return -1;
        }
        break;
      case 1:
        if (c == '/') {
          nivel--;
          if (nivel == 0) {
          free(obtener_lexema());
            return 0;
          }
          estado = 0;
        }
        else if (c == EOF) {
          free(obtener_lexema());
          alex_error_comentario_anidado(token, nivel);
          return -1;
        }
        else if (c == '+') estado = 1;
        else estado = 0;
        break;
      case 2:
        if (c == '+') {
            nivel++;
            estado = 0;
        }
        else if (c == EOF) {
          free(obtener_lexema());
          alex_error_comentario_anidado(token, nivel);
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
      case 0:
        if(c=='\\') estado = 1;
        else if(c=='"') estado = 2;
        else if (c == EOF || c == '\n') {
          free(obtener_lexema());
          alex_error_string_sin_cerrar(token);
          return -1;
        }
        else estado = 0;
        break;
      case 1:
        if (c == EOF) {
          free(obtener_lexema());
          alex_error_string_eof_escape(token);
          return -1;
        }
        estado = 0;
        break;
      case 2:
        devolver_se();
        token->lexema = obtener_lexema();
        token->comp_lexico = STRING;
        return 0;
    }
  c = siguiente_caracter();
  }
}

int _automata_binario(TOKEN * token, char c){
  int estado = 0;

  c = siguiente_caracter();
  while(1){
    switch (estado){
      case 0:
        if (c=='b' || c=='B') estado = 1;
        else {
          if (c!=EOF) devolver_se(); //retroceder el ultimo char
          return -1; //es un número decimal que empieza por 0
        }
        break;
      case 1:
        if (c=='0' || c=='1' || c=='_') estado = 2;
        else  { //no es un número binario bien formado pero de eso se ocupara el asin
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = ENTERO; //en el léxico de D los binarios son enteros
          return 0;
        }
        break;
      case 2:
        if (c=='0' || c=='1' || c=='_') estado = 2;
        else {
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = ENTERO; //en el léxico de D los binarios son enteros
          return 0;
        }
        break;
    }
    c = siguiente_caracter();
  }
}

int _automata_numero(TOKEN *token, char c, int estadoini){
  int estado = estadoini;

  while (1) {
    switch (estado) {
      case 0://parte entera
        if (isdigit(c) || c=='_') estado = 0;
        else if(c=='.') estado = 1;
        else if(c=='e' || c=='E') estado = 2;
        else{
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = ENTERO;
          return 0;
        }
        break;
      case 1:
        if (isdigit(c) || c=='_') estado = 1;
        else if (c=='e' || c=='E') estado = 2;
        else{
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
      case 2:
        if (isdigit(c) || c=='_'  || c=='+' || c=='-') estado = 3;
        else{ //mal formado pero lo aceptamos
          if (c!=EOF) devolver_se();
          token->lexema = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
      case 3:
        if (isdigit(c) || c=='_' ) estado = 3;
        else{ //mal formado pero lo aceptamos
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
      free(obtener_lexema());
      continue;
    }

    if (c==EOF){
      token->comp_lexico = EOF;
      token->lexema = NULL;
      return token;
    }

    if (isalpha(c) || c=='_'){
      int res = _automata_alphanum(token, c);
      if (res == 0) return token;
      continue;
    }

    if(isdigit(c)){
      if (c == '0') {
        int res = _automata_binario(token, c);
        if (res == 0) return token;
      }
      int res = _automata_numero(token, c, 0);
      if (res == 0) return token;
      continue;
    }

    if(c=='.'){
      c=siguiente_caracter();
      if (isdigit(c)){
        int res = _automata_numero(token, c, 1); //empieza en el estado 1 porque ya ha leído el .
        if (res == 0) return token;
      }
      if (c!=EOF) devolver_se();
      token->comp_lexico = (int)'.';
      token->lexema = obtener_lexema();
      return token;
    }

    if (c=='/'){
      c=siguiente_caracter();
      if (c=='/'){
        _automata_comentario_linea();
        continue;
      }
      else if (c=='*'){
        int res = _automata_comentario_multilinea(token);
        if (res==-1) return token;
        continue;
      }
      else if (c=='+'){
        int res = _automata_comentario_anidado(token);
        if (res==-1) return token;
        continue;
      }
      
      if (c!=EOF) devolver_se();
      token->comp_lexico = (int)'/';
      token->lexema = obtener_lexema();
      return token;
    }

    if(c=='='){
      c=siguiente_caracter();
      if(c=='=') token->comp_lexico = DOBLEIGUAL;
      else {
        if (c!=EOF) devolver_se();
        token->comp_lexico = (int)'=';
      }
      token->lexema = obtener_lexema();
      return token;
    }

    if(c=='+'){
      c=siguiente_caracter();
      if(c=='=') token->comp_lexico = MASIGUAL;
      else if(c=='+') token->comp_lexico = MASMAS;
      else {
        if (c!=EOF) devolver_se();
        token->comp_lexico = (int)'+';
      }
      token->lexema = obtener_lexema();
      return token;
    }
    
    if(c=='"'){
      _automata_string(token);
      return token; //que devuelva el token aunque haya fallo para que el asin vea el EOF
    }

    switch (c) {
      case ';': case '(': case ')': case '[':
      case ']': case '{': case '}': case ',':
      case '-': case '*': case '<':
        token->comp_lexico = (int)c;
        token->lexema = obtener_lexema();
        return token;
    }

    free(obtener_lexema());
    alex_error_caracter(token, (char)c);
    return token;
  }
}