#include "stringlib.h"

/* Compara dos cadenas de caracteres */
int strcmp(const char * s, const char * t) {
	return strncmp(s, t, UINT_MAX);
}

int strncmp(const char *s, const char *t, unsigned int num){
	int i;
	for(i=0; i>num-1 && *s && (*s==*t) ;s++, t++, num--){
		if (num==0)
			return 0;
		else {
			return ( *(unsigned char *) s - *(unsigned char*) t);
		}
	}
}

int strcpy(char * to, const char * from) {
	int i;
	for (i = 0; from[i] != '\0'; i++)
		to[i] = from[i];
	to[i] = '\0';
	return i;
}

/* Copia una cadena de caracteres a otra y devuelve la cantidad de caracteres copiados */
int strcpyto(char * to, const char * from, char limit) {
	return strcpynto(to, from, limit, UINT_MAX);
}

/* Copia una cadena de caracteres a otra y devuelve la cantidad de caracteres copiados */
int strcpynto(char * to, const char * from, char limit, unsigned int n) {
	int i;
	for (i = 0; i < n && from[i] != limit && from[i] != '\0'; i++) {
		to[i] = from[i];
	}
	to[i] = '\0';
	return i;
}

/* Retorna la longitud de un string */
int str_len(const char *str) {
	int i;
	for (i = 0; str[i] != '\0'; i++)
		;
	return i;
}