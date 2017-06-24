
#ifndef _DRIVERVIDEO_H_
#define _DRIVERVIDEO_H_

#include <stdint.h>

void printChar(char c);
void printString(char* string);
void clear();
unsigned char* get_vdcursor(void);
void set_vdcursor(unsigned char*);
void backspace();
void newline();
void scroll();
void selection(int finit, int cinit, int ffin, int cfin);
void undoSelection(int finit, int cinit, int ffin, int cfin);
void drawMouse(int f, int c);
void udrawMouse(int f, int c);
int validPosition(int f, int c);

#endif