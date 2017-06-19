#include "ctype.h"

/* retorna 1 si se recibe una letra o un digito */
int isalnum(int c) {
  return isalpha(c) || isdigit(c);
}

/* retorna 1 si se recibe una letra */
int isalpha(int c) {
  return islower(c) || isupper(c);
}

/* retorna 1 si se recibe un espacio ( o un tab )*/
int isblank(int c) {
  return c == '\t' || c == ' ';
}

/* retorna 1 si se recibe un caracter de control */
int iscntrl(int c) {
  return !isprint(c);
}

/* retorna 1 si se recibe un digito */
int isdigit(int c) {
  return c >= '0' && c <= '9';
}

/* retorna 1 si se recibe un caracter que se debe mostrar en pantalla o un espacio*/
int isgraph(int c) {
  return isprint(c) && c != ' ';
}

/* retorna 1 si se recibe una letra minúscula*/
int islower(int c) {
  return c >= 'a' && c <= 'z';
}

/* retorna 1 si se recibe un caracter imprimible*/
int isprint(int c) {
  return c > 0x1F && c != 0x7F;
}

/* retorna 1 si se recibe un signo de puntuacion*/
int ispunct(int c) {
  return isgraph(c) && !isalnum(c);
}

/* retorna 1 si se recibe un espacio en blanco*/
int isspace(int c) {
  return isblank(c) || c == '\v' || c == '\f' || c == '\r' || c == '\n';
}

/* retorna 1 si se recibe una letra mayúscula */
int isupper(int c) {
  return c >= 'A' && c <= 'Z';
}

/* retorna 1 si se recibe un digito hexadecimal*/
int isxdigit(int c) {
  return isdigit(c) || (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F');
}