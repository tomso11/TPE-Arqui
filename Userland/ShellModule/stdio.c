#include "ctype.h"
#include "stdlib.h"
#include "stringlib.h"
#include "stdio.h"
#include "syscalls.h"

//#include "driverVideo.h" //debug
//#include "naiveConsole.h"


#include <stdarg.h>

#define STDOUT 0
#define STDIN 1
#define STDERR 2
#define BUFF_SIZE 1024
#define AUX_SIZE 64
#define SPACE 0

static unsigned char buff[BUFF_SIZE];
static int index = 0;
static void * position = 0x7000000; //para malloc

/* Imprime un caracter en el descriptor indicado */
void fputchar(unsigned int fds, int c) {
	//char * buff;
	//*buff=c;
	write(fds, c, 1); //syscall
}

/* Imprime un caracter a salida estándar */
void putchar(int c) {
	fputchar(STDOUT, c);
}

void putstring(char * str){
	
	prints(STDOUT,str);
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
					//len += printi(fds, va_arg(args, int), aux);
					break;
				case 'i':
					//len += printi(fds, va_arg(args, int), aux);
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
    for (i = 0; i < maxlen-1 && (c = getchar(str)) != '\n'; i++) 
			str[i] = c;
    str[i] = '\0';
    return i;
}


int getchar(){
	char * getch=my_malloc(1);
	*getch=0;
	while (*getch == 0){
		read(STDIN, getch, 1);
	}
	if(*getch == '\b' || *getch == '\n'){
		//printString("bro");
	}
	else{
	//printChar(*getch);
	}	
	return (int)(unsigned char)*getch;
}


/* Llena el buffer hasta un '\n' o hasta que se termine su capacidad */
static void buffil() {

	unsigned char c;
	int i = 0;
	do{

		read(STDIN, &c, 1);

	} while ( c != '\n');

}


int usr_clear(){
	clear_screen();
}
