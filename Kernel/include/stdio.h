#ifndef _STDIO_H_
#define _STDIO_H_

#define STDOUT 0
#define STDIN 1
#define STDERR 2
#define BUFF_SIZE 1024
#define AUX_SIZE 64
#define SPACE 0

#include "ctype.h"
#include "stdlib.h"
#include "stringlib.h"
#include "stdio.h"

#include <stdarg.h>

void fputchar(unsigned int fds, int c);
void putchar(int c);
int getchar();
int printf(const char *format, ...);
int fprintf(unsigned int fds, const char *format, ...);
static int fprinttype(unsigned int fds, const char *format, va_list args);
static int printi(unsigned int fds,int value, char aux[]);
static int prints(unsigned int fds, const char *str);
int readRow(char *str, unsigned int maxlen);
int readRowAndClear(char *str, unsigned int maxlen);
int scanf(const char *format, ...);
static void buffil();

#endif