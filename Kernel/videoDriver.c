// recordemos que la pantalla enrealidad es una matriz de 160x25 guardados en espacios contiguos de memoria
// en realidad utilizamos solo 80 y nos movemos de a dos ya que el segundo byte es para el formato (color y bg color)

#include "keyboardDriver.h"


static unsigned int  bufferLength= 20; // arbitrario
static char buffer[20];
static unsigned char * const video = (unsigned char*)0xB8000;
static unsigned char * currentVideo = (unsigned char*)0xB8000;
static const unsigned int width = 80;
static const unsigned int height = 25 ;

void printString(const char * string){
	int i;

	for (i = 0; string[i] != 0; i++)
		printChar(string[i]);
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

	for(i=0; i < height ; i++ ){
		video[80 * 2 * line + i * 2]= ' ' ;
	}
	currentVideo=video+80 * 2 * (line-1); 
}

void printChar(char character){
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
}

void insertLine(){

	do{
		printChar(' ');
	}while((unsigned char)(currentVideo - video) % (width * 2) != 0);
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