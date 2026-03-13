#ifndef TOKEN_H
#define TOKEN_H

/*
  Estructura para los tokens que intercambian el analizador lexico y el sintáctico.
*/
typedef struct {
    int comp_lexico; //Código numérico correspondiente al componente léxico
    char *lexema; //Lexema
} TOKEN;

#endif /* TOKEN_H */