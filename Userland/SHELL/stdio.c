#include "stdio.h"
#include "stdioASM.h"
#define LETTER  0
#define FORMAT  1
#define NULL 0
#define BUFFERSIZE 2500


int putchar(char c){
	char * msg = malloc(1);
	*msg = c;
	writeC(msg,1);
	return 0;
}

char getchar(){
	char * buffer = malloc(1);
	*buffer = 0;
	while( *buffer == 0){
		readC(buffer,1);
	}

	return *buffer;
}


void putNumber(int n){
	char * buffer = malloc(20);
	int size = 0;
	if(n < 0){
		putchar('-');
		n = -n;
	}
	while(n/10){
		*buffer = n%10 +'0';
		buffer++;
		size++;
		n = n/10;
	}
	*buffer = n + '0';
	size++;
	while(size != 0){
		putchar(*buffer);
		buffer--;
		size--;
	}
	
}


void printFF(const char * format, char ** s, int * n){
 int state = LETTER;
 while(*format != 0){
 	switch(state){

		case LETTER:
			if(*format == '%'){
				state = FORMAT;
				format++;
				break;
			}

			putchar(*format);
			format++;
			break;
		case FORMAT:
			if(*format == 'd'){
				if(n != NULL){
					int nummber = *n;
				n++;
				putNumber(nummber);
				state = LETTER;
				format++;
				break;
				
				}
				
			}
			if(*format == 's'){
				if(s != NULL){
					char * string = s[0];
				s++;
				while(*string != 0){
					putchar(*string);
					string++;
				}
				state = LETTER;
				format++;
				break;
				
				}
				
			}
			else {
				putchar('%');
				format++;
				break;
			}

		}
	}
	if(*(format-1) == '%'){
		putchar('%');
	}
	return;

}

int scanFF(const char * format, char ** s, int * n){
 int bufferindex;
 int state = LETTER;
 int indexS = 0;
 int indexN = 0;
 while(*format != 0){
 	switch(state){

		case LETTER:
			if(*format == '%'){
				state = FORMAT;
				format++;
				break;
			}
			char c;
			while((c=getchar()) == *format){
				;
			}
			format++;
			break;
		case FORMAT:
			if(*format == 'd'){
				if(n != NULL){
					int nummber = n[indexN];
				n++;
				indexN++;
				putNumber(nummber);
				state = LETTER;
				format++;
				break;
				
				}
				
			}
			if(*format == 's'){
				if(s != NULL){
				char * string = s[indexS];
				indexS++;
				char c;
				bufferindex = 0;
				while((c = getchar()) != '\n'){
					if(c == '\b'){
						if(bufferindex != 0){
							string--;
							bufferindex--;
						}
					}
					else{
						if(bufferindex == BUFFERSIZE){
							break;
						}
						*string = c;
						string++;
						bufferindex++;
					}	
				}
				*string = 0;
				state = LETTER;
				format++;
				break;
				
				}
				
			}
		}
	}
	return bufferindex == BUFFERSIZE;

}

void * malloc(int bytes){
	static void * position = 0x1000000;
	void * aux = position;
	position+=bytes;
	return aux;
}

int strcmp(char * str1, char * str2){
	while((*str1 != 0) && (*str2 != 0)){
		if((*str1 - *str2) > 0){
			return 1;
		}
		if((*str1 - *str2) < 0){
			return -1;
		}
		str1++;
		str2++;
	}
	if(*str1 == 0 && *str2 == 0){
		return 0;
	}
	if(*str1 == 0){
		return -1;
	}
	return 1;
}

int strcmpN(char * str1, char * str2,int number){
	while((*str1 != 0) && (*str2 != 0) && number){
		if((*str1 - *str2) > 0){
			return 1;
		}
		if((*str1 - *str2) < 0){
			return -1;
		}
		str1++;
		str2++;
		number--;
	}
	if(!number){
		return 0;
	}
	if(*str1 == 0 && *str2 == 0){
		return 0;
	}
	if(*str1 == 0){
		return -1;
	}
	return 1;
}




