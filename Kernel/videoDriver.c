// recordemos que la pantalla enrealidad es una matriz de 160x25 guardados en espacios contiguos de memoria
// en realidad utilizamos solo 80 y nos movemos de a dos ya que el segundo byte es para el formato (color y bg color)

#include "keyboardDriver.h"
#include "interruptions.h"
#include <naiveConsole.h>


static unsigned int  bufferLength= 20; // arbitrario
static char buffer[20];
static unsigned char * const video = (unsigned char*)0xB8000;
static unsigned char * currentVideo = (unsigned char*)0xB8000;
static const unsigned int width = 80;
static const unsigned int height = 25;
static unsigned char cursor_x=0;
static unsigned char cursor_y=0;

void printString(const char * string){
	int i;

	for (i = 0; string[i] != 0; i++)
		printChar(string[i]);
	
}

void initialize_cursor(){
	update_cursor(0, 0);
}



void printChar(char character){
	
	*currentVideo = character;
	currentVideo += 2;

}


void clear(){
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}


 /* void update_cursor(int row, int col)
  * by Dark Fiber
  */
 void update_cursor(int row, int col)
 {
 	int x=cursor_x+col;
 	int y=cursor_y+row;
 	if(x>width*2){
 		x=80;
 	}
 	if(y>height){
 		y=25;
 	}
 	if(x<0){
 		x=0;
 	}
 	if(y<0){
 		y=0;
 	}
    unsigned short position=(y*80) + x;
 
    // cursor LOW port to vga INDEX register
    write_port(0x3D4, 0x0F);
    write_port(0x3D5, (unsigned char)(position&0xFF));
    // cursor HIGH port to vga INDEX register
    write_port(0x3D4, 0x0E);
    write_port(0x3D5, (unsigned char )((position>>8)&0xFF));
    cursor_x=x;
    cursor_y=y;
 }


unsigned char* get_vdcursor(){
	return currentVideo;
}

void set_vdcursor(unsigned char* ptr){
	currentVideo=ptr;
}

void backspace(){
	currentVideo=currentVideo-2;
	printChar(' ');
	currentVideo=currentVideo-2;
	return;
}

void newline(){

	do{
		printChar(' ');
	}while( ((currentVideo-video) % (width*2)) != 0 );
	unsigned char* aux=currentVideo;
	do{
		printChar(' ');
	}while( ((currentVideo-video) % (width*2)) != 0 );
	currentVideo=aux;
	return;
}
