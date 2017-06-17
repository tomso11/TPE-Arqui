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
// static unsigned char * curr_row=(unsigned char) (currentVideo/(unsigned char)width);
// static unsigned char * curr_col=(unsigned char) (currentVideo%(unsigned char)width);
#define ROW(c) ( ((c-video) - ( (c-video) % ((width)*2) ) )/ ((width)*2) )
#define COL(c) ((c-video) % (width*2))
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



void scroll(){
	int line = 24;
	char * lineText;
	char * aux;

	while ( line >= 0 ){
		copyLine(aux, line);
		eraseLine(line);
		if( lineText != 0 ){
			printString( lineText );
		}
		line--;
	}

}

void copyLine(char * aux, int line){
	int i;
	for( i=0; i< width; i++){
		aux[i]= (char) video + 80* 2 * line + i * 2; // video + 160*line nos lleva a la linea que buscamos y i*2 nos lleva al caracter.
	}
}

void eraseLine(int line){
	int i;


	for(i=0; i < width ; i++ ){
		video[80*2*line+i*2]= ' ' ;
	}
	currentVideo=video+80 * 2 * (line-1); 
}

void printChar(char character){
	if( ((currentVideo-video) % (width*2)) == 0 && ( (currentVideo-video) != 0 ) ){
		//insertLine(); // terminar de implementar
	}
	if( ((currentVideo - video) % ((width*2) * (height) )) == 0 && ( (currentVideo-video) != 0 ) ){
		eraseLine(0);
		currentVideo=video;
	} 
	 if(character == '\n'){
		insertLine();
		}else{
	 	if(character == '\b'){
			currentVideo=currentVideo-2;
			*currentVideo=' ';
		}else{
			*currentVideo = character;
			currentVideo += 2;
		}
	 }
	 // update_cursor(ROW((currentVideo-1)), COL((currentVideo-1)));   // lo hacemos mas adelante
}

void insertLine(){
	unsigned char * video_aux=currentVideo;
	int i=0;

	for (; ((unsigned char)(video_aux+(i*2) - video) % (width * 2) != 0 ); i++){
		//video_aux+(unsigned char)i*2=' ';
	}
	// do{
	// 	printChar(' ');
	// }while((unsigned char)(currentVideo - video) % (width * 2) != 0);
}

void clear(){
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}


void console(){
	char aux='0';
	printString("It Works!");
	while(1){
		aux=get_char();
		if( aux != -1){
			printString("yes");
		}
	}
}

void testfoo(){
	printString("this works");
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

 void asm_cursor(){
 	cursor();
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