#include "ctype.h"
#include "stdlib.h"
#include "stringlib.h"
#include "stdio.h"
#include "syscalls.h"

#include "driverVideo.h" //debug
#include "naiveConsole.h"


#include <stdarg.h>

#define STDOUT 0
#define STDIN 1
#define STDERR 2
#define BUFF_SIZE 1024
#define AUX_SIZE 64
#define SPACE 0

static unsigned char buff[BUFF_SIZE];
static int index = 0;

/* Imprime un caracter en el descriptor indicado */
void fputchar(unsigned int fds, int c) {
	write(fds, &c, 1); //syscall
}

/* Imprime un caracter a salida estándar */
void putchar(int c) {
	fputchar(STDOUT, c);
}


int printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	return fprinttype(STDOUT, format, args);
}

int fprintf(unsigned int fds, const char *format, ...) {
	va_list args;
	va_start(args, format);
	return fprinttype(fds, format, args);
}

/* Imprime en el descriptor fds con el formato indicado. Soporta los siguientes: d,i,s,c. Retorna la cantidad de caracteres escritos.
** En caso de simbolo invalido imprime todos los caracteres hasta la ocurrencia de dicho simbolo donde deja de imprimir y retorna.*/
static int fprinttype(unsigned int fds, const char *format, va_list args) {
	int len = 0; // cantidad de caracteres escritos
	char aux[AUX_SIZE];

	while(*format != '\0') {
		char c = *format++;
		char symbol;
		if(c != '%') {
			fputchar(fds,c);
			len++;
		}
		else {
			symbol = *format++;
			switch(symbol) {
				case 'd':
					len += printi(fds, va_arg(args, int), aux);
					break;
				case 'i':
					len += printi(fds, va_arg(args, int), aux);
					break;
				case 's':
					len += prints(fds, va_arg(args, char *));
					break;
				case 'c':
					fputchar(fds, va_arg(args,int) & 0xFF);
					len++;
					break;
				case '%':
					fputchar(fds, '%');
					len++;
					break;
				default:  // simbolo invalido. Termina la funcion y retorna
					return len;
			}
		}
	}
	va_end(args);
	return len;
}


/*Imprime un número entero en la pantalla */
static int printi(unsigned int fds,int value, char aux[]) {
	itoa(value, aux, 10); // guarda en buffer el string del valor en base 10
	return prints(fds, aux);
}

/*Imprime una cadena de carateres a pantalla */
static int prints(unsigned int fds, const char *str) {
	int i;
	for (i=0; str[i] != '\0'; i++)
		fputchar(fds, str[i]);
	return i;
}

/* Lee una línea de entrada estándar de longitud como mucho maxlen y la guarda en str.*/
int readRow(char *str, unsigned int maxlen) {
    unsigned int i;
    int c;
    for (i = 0; i < maxlen-1 && (c = getchar()) != '\n'; i++) 
			str[i] = c;
    str[i] = '\0';
    return i;
}

/* Lee toda una linea de input y limpia si queda un espacio al final. */
int readRowAndClear(char *str, unsigned int maxlen) {
	unsigned int i = 0;
	int c;
	char state = SPACE;
	while ((c = getchar()) != '\n' && i < maxlen-1) {
		if (state != SPACE) {
			str[i++] = c;
			if (isspace(c))
				state = SPACE;
		}
		else if (!isspace(c)) {
			str[i++] = c;
			state = !SPACE;
		}
	}
	if (i > 0 && isspace(str[i-1]))
		i--;  // Se borra el utimo espacio si lo hay
	str[i] = '\0';
	return i;
}


/*Lee del buffer hasta '\n' caso en el cual lo marca como vacio. Si esta vacio el buffer lo llena.*/
int getchar() {
	unsigned char * getch;
	*getch=0;
	while (*getch == 0){
		read(STDIN, getch, 1);
	}
	//printChar(*getch);
	if(*getch=='\0'){
		return -1;
	}

	
	// if (index == 0) // buffer vacio
	// 	buffil();
 //    unsigned char c = buff[index++]; // Se lee caracter del buffer
 //    if (c == '\n' || index== BUFF_SIZE)
 //    	index = 0;  // reset buffer
    return *getch;
}

/* Llena el buffer hasta un '\n' o hasta que se termine su capacidad */
static void buffil() {

	unsigned char c;
	int i = 0;
	do{

		read(STDIN, &c, 1);

	} while ( c != '\n');

}

/*Implementacion de scanf que acepta los simbolos d y s.*/
int scanf(const char *format, ...) {
	int i, j, num;
	int argc = 0;
	int *p;
	char *str;
	char aux[BUFF_SIZE];
	va_list args;
	va_start(args, format);

	readRow(aux, BUFF_SIZE);

	for (i = j = 0; aux[j] != '\0' && format[i] != '\0'; i++, j++) {
		if (format[i] == '%') {
			i++;
			if (format[i] == 'd') {
				if(aux[j] != '+' && aux[j] != '-' && !isdigit(aux[j]))
					return argc;
				num = atoi(aux + j);
				p = va_arg(args, int *);
				*p = num;
				while (isdigit(aux[j+1]))
					j++;
			}
			else if (format[i] == 's') {
				str = va_arg(args, char *);
				strcpy(str, aux+j);
				return argc+1;
			}
			else if (format[i] == '%' && aux[j++] != '%')
				return argc;
			argc++;
		} 
		else if (format[i] != aux[j])
			return argc;
	}

	va_end(args);
	return argc;
}

int usr_clear(){
	clear();
}


