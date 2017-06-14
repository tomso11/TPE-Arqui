#ifndef _DRIVERKEYBOARD_H_
#define _DRIVERKEYBOARD_H_
	
	#define CHARCODE_MAX 128
	#define BUFFCAP 128
	#define TRUE 1
	#define FALSE 0
	#define ESCAPE 1
	#define RSHIFTCODE 42
	#define LSHIFTCODE 54
	#define CAPSLOCK 58
	

	void addToBuffer();
	int get_char();
	int chooseKeyboard(int k);
	int specialKeys(int k);
	char pollBuffer();

#endif