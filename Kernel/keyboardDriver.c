#include "scancodes.h"
#include "keyboardDriver.h"


#define CHARCODE_MAX 128
#define BUFFCAP 128
#define TRUE 1
#define FALSE 0
#define ESCAPE 1
#define RSHIFTCODE 42
#define LSHIFTCODE 54
#define CAPSLOCK 58

static char buffer[BUFFCAP];
static unsigned int ret_index=0;
static unsigned int store_index=0;
static int buffsize = 0;
static int shift=FALSE;
static int caps=FALSE;
static int capsActive=FALSE;
static int capsCount=0;

char get_key();

void addToBuffer(){
	
	// Si esta lleno el buffer no podemos guardar
	// podemos hacer un buffer circular ( a implementar dsp )
	if (buffsize == BUFFCAP){
		return;
	}

	int k=get_key(); //en asm ---> GLOBAL read_key  read_key: in al, 60h     ret

  	//si es un scancode valido
	if(k>0 && k<CHARCODE_MAX){
		//si es un caracter
		if( !specialKeys(k) ){
			buffsize++;
			buffer[store_index++]= k;
		}	
	
	}else if(k<0){
		specialKeys(k + CHARCODE_MAX);
	}	//es el scancode de cuando soltas una tecla
	
	//si se lleno el buffer
	if (store_index == BUFFCAP){
    	store_index = 0;	
    }
}

//tenemos esta funcion para interpretar la tecla segun los flags q tengamos activados
int get_char(){
	if(buffsize == 0)
		return -1;
		
	char k=buffer[ret_index++];
	buffsize--;
	
	if (ret_index == BUFFCAP){
        ret_index = 0;
	}    

	return chooseKeyboard(k);
}

int chooseKeyboard(int k){

	if (keybnor[k] == 0){ // no es imprimible
    	return -1;
    }

	if(!capsActive && !shift){
    	return keybnor[k];
	}

	if(capsActive && !shift ){
    	return keybcaps[k];
	}

	if(!capsActive && shift){
    	return keybshft[k];
	}

	if(capsActive && shift){
    	return keybshftcaps[k];
	}

}


int specialKeys(int k){

	switch(k){

    	case RSHIFTCODE:
          shift=!shift;
          return TRUE;
          break;

    	case LSHIFTCODE:
          shift=!shift;
          return TRUE;
          break;

    	case CAPSLOCK:
          caps = !caps;
          capsCount += 1;
          if(capsCount % 2 == 0) { //fue apretado y soltado
            capsActive = !capsActive;
            capsCount = 0;
          }
          return TRUE;
          break;

    	default:
          return FALSE;
          break;

  	}
  
  	return FALSE;
}

char pollBuffer(){
	if(ret_index == 0){
		return '0';
	}
	return buffer[ret_index--];
}