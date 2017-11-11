#include "ctype.h"

/* Retorna 1 si el entero recibido como parámetro es una letra o un digito */
int isalnum(int c) {
  return isalpha(c) || isdigit(c);
}

/* Retorna 1 si el entero recibido como parámetro es una letra */
int isalpha(int c) {
  return islower(c) || isupper(c);
}

/* Retorna 1 si el entero recibido como parámetro es un espacio ( o un tab )*/
int isblank(int c) {
  return c == '\t' || c == ' ';
}

/* Retorna 1 si el entero recibido como parámetro es un caracter de control */
int iscntrl(int c) {
  return !isprint(c);
}

/* Retorna 1 si el entero recibido como parámetro es un digito */
int isdigit(int c) {
  return c >= '0' && c <= '9';
}

/* Retorna 1 si el entero recibido como parámetro es un caracter que se debe mostrar en pantalla o un espacio*/
int isgraph(int c) {
  return isprint(c) && c != ' ';
}

/* Retorna 1 si el entero recibido como parámetro es una letra minúscula*/
int islower(int c) {
  return c >= 'a' && c <= 'z';
}

/* Retorna 1 si el entero recibido como parámetro es un caracter imprimible*/
int isprint(int c) {
  return c > 0x1F && c != 0x7F;
}

/* Retorna 1 si el entero recibido como parámetro es signo de puntuacion*/
int ispunct(int c) {
  return isgraph(c) && !isalnum(c);
}

/* Retorna 1 si el entero recibido como parámetro es un espacio en blanco*/
int isspace(int c) {
  return isblank(c) || c == '\v' || c == '\f' || c == '\r' || c == '\n';
}

/* Retorna 1 si el entero recibido como parámetro es una letra mayúscula */
int isupper(int c) {
  return c >= 'A' && c <= 'Z';
}

/* Retorna 1 si el entero recibido como parámetro es un digito hexadecimal*/
int isxdigit(int c) {
  return isdigit(c) || (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F');
}
