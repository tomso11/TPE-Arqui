// recordemos que la pantalla enrealidad es una matriz de 160x25 guardados en espacios contiguos de memoria
// en realidad utilizamos solo 80 y nos movemos de a dos ya que el segundo byte es para el formato (color y bg color)

#include "driverKeyboard.h"
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
static char mouse_buffer[4000];

void printString(const char * string){
	int i;

	for (i = 0; string[i] != 0; i++)
		printChar(string[i]);
	
}

void initialize_cursor(){

	update_cursor(0, 0);
	
}


/* Escribe en el video. Realiza los chequeos correspondientes al limite de la pantalla*/

void printChar(char character){
	if( currentVideo == video+4000 ){
		scroll();
	}
	if( (currentVideo!=video) && ((currentVideo-video) % (width*2) == 0) ){
		newline();
	}
	if (character == '\b'){
		backspace();
		return;
	}
	if( character == '\n'){
		newline();
		return;
	}
	else{
		*(currentVideo+1)=0x0F;
	}
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
 	if(x>width){
 		x=80;
 	}
 	if(y>height){
 		y=24;
 	}
 	if(x<0){
 		x=0;
 	}
 	if(y<0){
 		y=0;
 	}
    
 	udrawMouse(cursor_y,cursor_x);
 	drawMouse(x,y);
 	// printString("\nMoved from:");
 	// ncPrintDec(cursor_x);
 	// printChar(' ');
 	// ncPrintDec(cursor_y);
 	// printChar('\n');
 	// printString("to:");
 	// ncPrintDec(x);
 	// printChar(' ');
 	// ncPrintDec(y);
 	// printChar('\n');
    // // cursor LOW port to vga INDEX register
    // write_port(0x3D4, 0x0F);
    // write_port(0x3D5, (unsigned char)(position&0xFF));
    // // cursor HIGH port to vga INDEX register
    // write_port(0x3D4, 0x0E);
    // write_port(0x3D5, (unsigned char )((position>>8)&0xFF));
    cursor_x=x;
    cursor_y=y;

 }

 // void initialize_screen(){
	// int i;
 // 	for(i=1; i<4000 ; i=i+2){
 // 		*(video+i)=0x0F;
 // 	}
 // }

/* Realiza la seleccion del mouse. */

void selection(int finit, int cinit, int ffin, int cfin){
 
	int i= finit*width+ cinit;
	int fin= ffin*width+cfin;
	copyMouse(i,fin);
	while(validPosition(i/width,i%width) && i<=fin ){

		drawMouse(i/width,i%width);
		i++;
	}
}

/* Borra la seleccion del mouse una vez realizada la accion con esta */

void undoSelection(int finit, int cinit, int ffin, int cfin){
	int i= finit*width+ cinit;
	int fin= ffin*width+cfin;
	while(validPosition(i/width,i%width) && i<=fin ){
		
		udrawMouse(i/width,i%width);
		i++;
	}

} 

/*dibuja el cursor*/

void drawMouse(int c, int f){
	if(!validPosition(c,f)) return;
		// printString("\nDrawing to:");
		// ncPrintDec(c*2);
		// printChar(' ');
		// ncPrintDec(f*79*2);
		// printChar('\n');
	*(video+(c*2)+(f*79*2)+1)=(char) 0x30; // cursor celeste
}
void udrawMouse(int f, int c){
	//if(!validPosition(f,c)) return;
	*(video+(c*2)+(f*79*2)+1)=(char) 0x0F; // fondo negro con blanco como texto
}

void copyMouse(int initial, int end){
	int size=end-initial+1;
	int i;
	int n=0;
	for(i=0; i<size; i++){
		mouse_buffer[n++]=*(video+end-i*2);
	}
	mouse_buffer[n]='\0';
	return;
}

void paste(){
	int i;
	for (i=0; mouse_buffer[i]!= '\0' ; i++){
		printChar(mouse_buffer[i]);
	}
	mouse_buffer[0]='\0';
	return;
}

/* Chequea que sea una posicion valida en pantalla para realizar la copia*/

int validPosition(int f, int c){
	return (f>=width || c>=width || f<0 || c<0)? 0:1;
}

/* DEPRECATED */

unsigned char* get_vdcursor(){
	return currentVideo;
}

/* DEPRECATED */

void set_vdcursor(unsigned char* ptr){
	currentVideo=ptr;
}

/* Realiza el backspace siempre y cuando no este en la primera posicion */

void backspace(){
	if(currentVideo == video){
		return;
	}
	currentVideo=currentVideo-2;
	*currentVideo=' ';
	return;
}

/* Realiza el newline teniendo en cuenta si se pasa la ultima linea, si sucede, realiza un scroll */

void newline(){

	while( ((currentVideo-video) % (width*2)) != 0 ){ // limpia la linea actual hasta llegar al final de linea
		*currentVideo=' ';
		currentVideo=currentVideo+2;
	}
	if( currentVideo == video+4000 ){ // si llegamos a final de pantalla hacemos scroll
		scroll();
	}
	unsigned char* aux=currentVideo;

	do{
		*currentVideo=' ';
		currentVideo=currentVideo+2;
	}while( ((currentVideo-video) % (width*2)) != 0 ); //limpiamos la linea siguiente
	currentVideo=aux;
	return;
}



void scroll(){
	unsigned char * act= video;
	unsigned char * flwing= act+80;
	int i;
	for(i=80 ; i<4000; i++){
		act[(i-80)]=flwing[i];
	}
	currentVideo=act+4000-160;
	return;

}