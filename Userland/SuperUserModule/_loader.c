/* _loader.c */
#include <stdint.h>

extern char bss;
extern char endOfBinary;

int main();

void * memset(void * destiny, int32_t c, uint64_t length);

int _start() {
	char * v=0xB8000; //try
	int i;
	for(i=0;i<4000; i++)
		*(v+i)='x';
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	return main();

}


void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
