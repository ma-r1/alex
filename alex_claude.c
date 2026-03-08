#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "alex.h"
#include "errores.h"
#include "se.h"
#include "ts.h"
#include "definiciones.h"

/* ─────────────────────────────────────────────
 * Convención de retorno de los autómatas:
 * 0  → token relleno correctamente
 * -1  → error léxico; el token ya contiene EOF
 * ───────────────────────────────────────────── */

int _automata_alphanum(TOKEN *token, char c) {
  int estado = 0;

  while (1) {
    switch (estado) {
      case 0:
        if (isalnum(c) || c == '_') {
          c = siguiente_caracter();
        } else {
          estado = 1;
        }
        break;
      case 1:
        devolver_se();
        token->lexema      = obtener_lexema();
        token->comp_lexico = buscar_ts(token->lexema);
        if (token->comp_lexico == -1) {
          token->comp_lexico = ID;
          insertar_ts(token->lexema);
        }
        return 0;
    }
  }
}

int _automata_comentario_linea(void) {
  char c = siguiente_caracter();
  while (c != '\n' && c != EOF) {
    c = siguiente_caracter();
  }
  obtener_lexema();
  return 0;
  /* EOF al final de línea se trata como fin del comentario:
     el bucle principal lo detectará en la siguiente iteración. */
}

int _automata_comentario_multilinea(TOKEN *token) {
  int estado = 0;
  int error  = 0;
  int fin    = 0;
  char c     = siguiente_caracter();

  while (!fin && !error) {
    switch (estado) {
      case 0:
        if      (c == '*')  estado = 1;
        else if (c == EOF)  error  = 1;
        break;
      case 1:
        if      (c == '/')  fin    = 1;
        else if (c == EOF)  error  = 1;
        else if (c != '*')  estado = 0;
        break;
    }
    if (!fin && !error) c = siguiente_caracter();
  }

  obtener_lexema();
  if (error) {
    alex_error_comentario_multilinea(token);
    return -1;
  }
  return 0;
}

int _automata_comentario_anidado(TOKEN *token) {
  int estado = 0;
  int nivel  = 1;
  int error  = 0;
  int fin    = 0;
  char c     = siguiente_caracter();

  while (!fin && !error) {
    switch (estado) {
      case 0:
        if      (c == '+')  estado = 1;
        else if (c == '/')  estado = 2;
        else if (c == EOF)  error  = 1;
        break;
      case 1:  /* posible cierre '+/' */
        if (c == '/') {
          if (--nivel == 0) fin = 1;
          else              estado = 0;
        } else if (c == EOF) {
          error = 1;
        } else if (c != '+') {
          estado = 0;
        }
        break;
      case 2:  /* posible apertura '/+' */
        if (c == '+') {
          nivel++;
          estado = 0;
        } else if (c == EOF) {
          error = 1;
        } else if (c != '/') {
          estado = 0;
        }
        break;
    }
    if (!fin && !error) c = siguiente_caracter();
  }

  obtener_lexema();
  if (error) {
    alex_error_comentario_anidado(token, nivel);
    return -1;
  }
  return 0;
}

int _automata_string(TOKEN *token) {
  int  estado = 0;
  char c      = siguiente_caracter();

  while (1) {
    switch (estado) {
      case 0:
        if      (c == '\\')                    estado = 1;
        else if (c == '"')                     estado = 2;
        else if (c == EOF || c == '\n') {
          obtener_lexema();
          alex_error_string_sin_cerrar(token);
          return -1;
        }
        break;
      case 1:
        if (c == EOF) {
          obtener_lexema();
          alex_error_string_eof_escape(token);
          return -1;
        }
        estado = 0;
        break;
      case 2:
        devolver_se();
        token->lexema      = obtener_lexema();
        token->comp_lexico = STRING;
        return 0;
    }
    c = siguiente_caracter();
  }
}

int _automata_binario(TOKEN *token, char c) {
  int estado = 0;
  c = siguiente_caracter();

  while (1) {
    switch (estado) {
      case 0:
        if (c == 'b' || c == 'B') {
          estado = 1;
        } else {
          devolver_se();
          return -1;  /* decimal que empieza por 0, continúa en _automata_numero */
        }
        break;
      case 1:
        if (c == '0' || c == '1' || c == '_') {
          estado = 2;
        } else {
          devolver_se();
          token->lexema      = obtener_lexema();
          token->comp_lexico = ENTERO;
          return 0;
        }
        break;
      case 2:
        if (c == '0' || c == '1' || c == '_') {
          /* seguimos */
        } else {
          devolver_se();
          token->lexema      = obtener_lexema();
          token->comp_lexico = ENTERO;
          return 0;
        }
        break;
    }
    c = siguiente_caracter();
  }
}

int _automata_numero(TOKEN *token, char c) {
  int estado = 0;

  while (1) {
    switch (estado) {
      case 0:  /* parte entera */
        if      (isdigit(c) || c == '_') { /* seguimos */ }
        else if (c == '.')               estado = 1;
        else if (c == 'e' || c == 'E')   estado = 2;
        else {
          devolver_se();
          token->lexema      = obtener_lexema();
          token->comp_lexico = ENTERO;
          return 0;
        }
        break;
      case 1:  /* parte decimal */
        if      (isdigit(c) || c == '_') { /* seguimos */ }
        else if (c == 'e' || c == 'E')   estado = 2;
        else {
          devolver_se();
          token->lexema      = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
      case 2:  /* exponente: signo opcional */
        if (isdigit(c) || c == '_' || c == '+' || c == '-') {
          estado = 3;
        } else {
          devolver_se();
          token->lexema      = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
      case 3:  /* exponente: dígitos */
        if (isdigit(c) || c == '_') { /* seguimos */ }
        else {
          devolver_se();
          token->lexema      = obtener_lexema();
          token->comp_lexico = DECIMAL;
          return 0;
        }
        break;
    }
    c = siguiente_caracter();
  }
}

/* ─────────────────────────────────────────────
 * Punto de entrada del analizador léxico
 * ───────────────────────────────────────────── */
TOKEN *siguiente_componente_lexico() {
  TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
  int c;

  while (1) {
    c = siguiente_caracter();

    /* Espacios en blanco */
    if (c == ' ' || c == '\n' || c == '\t' || c=='\r') {
      obtener_lexema();
      continue;
    }

    /* Fin de fichero */
    if (c == EOF) {
      alex_eof_token(token);
      return token;
    }

    /* Identificadores y palabras clave */
    if (isalpha(c) || c == '_') {
      if (_automata_alphanum(token, c) == 0) return token;
      continue;
    }

    /* Números */
    if (isdigit(c)) {
      if (c == '0' && _automata_binario(token, c) == 0) return token;
      if (_automata_numero(token, c) == 0) return token;
      continue;
    }

    /* Comentarios y división */
    if (c == '/') {
      c = siguiente_caracter();
      if (c == '/') {
        _automata_comentario_linea();
        continue;
      }
      if (c == '*') {
        if (_automata_comentario_multilinea(token) == -1) return token;
        continue;
      }
      if (c == '+') {
        if (_automata_comentario_anidado(token) == -1) return token;
        continue;
      }
      devolver_se();
      token->comp_lexico = (int)'/';
      token->lexema      = obtener_lexema();
      return token;
    }

    /* Operadores compuestos y simples */
    if (c == '=') {
      c = siguiente_caracter();
      if (c == '=') {
        token->comp_lexico = DOBLEIGUAL;
      } else {
        devolver_se();
        token->comp_lexico = (int)'=';
      }
      token->lexema = obtener_lexema();
      return token;
    }

    if (c == '+') {
      c = siguiente_caracter();
      if      (c == '=') token->comp_lexico = MASIGUAL;
      else if (c == '+') token->comp_lexico = MASMAS;
      else {
        devolver_se();
        token->comp_lexico = (int)'+';
      }
      token->lexema = obtener_lexema();
      return token;
    }

    /* String */
    if (c == '"') {
      if (_automata_string(token) == -1) return token;  /* token ya es EOF */
      return token;
    }

    /* Símbolos de un solo carácter */
    switch (c) {
      case '.': case ';': case '(': case ')':
      case '[': case ']': case '{': case '}':
      case ',': case '-': case '*': case '<':
        token->comp_lexico = (int)c;
        token->lexema      = obtener_lexema();
        return token;
    }

    /* Carácter no reconocido */
    obtener_lexema();
    alex_error_caracter(token, (char)c);
    return token;
  }
}