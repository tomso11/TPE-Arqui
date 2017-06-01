
#include "scancodes.h"

#define BUFFCAP 100

extern char read_key();
static char buffer[BUFFCAP];
static unsigned int buffsize=0;
static unsigned int current_pos=0;
static int shift_pressed=FALSE;
static int shift_pressed=FALSE;
static int ctrl_pressed=FALSE;
static int capslock_times_pressed = 0;
static int capslock_active = FALSE;



void save_scancode(){

	// Si esta lleno el buffer no podemos guardar
	if (buffsize == BUFFCAP){
		return;
	}

	int k=read_key(); //en asm

	//si es un scancode valido
	if(k>0 && k<CHARCODE_MAX){
		//si es un caracter
		if( characterKey() ){
			buffsize++;
			buffer[current_pos++]= k;

		}
	}else if(k<0){
		//es el scancode de cuando soltas una tecla
	}

	//si se lleno el buffer
	if(current_pos == BUFFCAP){
		current_pos =0 ;
	}
}

int get_char(){
	if(buffsize == 0)
		return -1;
	char k=buffer[current_pos--];
	buffsize--;
	//tenemos esta funcion para interpretar la tecla segun los flags q tengamos activados
	return keyValue(k);
}

int keyValue(int k){

	if( keyboard[k] == 0 ){
		return -1;
	}

	//manejar dependiendo de cada uno
	if(capslock_active && !shift_pressed && !ctrl_pressed){

	}
	if(capslock_active && shift_pressed && !ctrl_pressed){
		
	}
	if(capslock_active && shift_pressed && ctrl_pressed){
		
	}
	if(capslock_active && !shift_pressed && ctrl_pressed){
		
	}
	if(!capslock_active && shift_pressed && !ctrl_pressed){
		
	}
	if(!capslock_active && !shift_pressed && ctrl_pressed){
		
	}
	if(!capslock_active && shift_pressed && ctrl_pressed){
		
	}
	if(!capslock_active && shift_pressed && ctrl_pressed){
		
	}
	return keyboard[k];
}
