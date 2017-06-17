
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

#endif