#include <interruptions.h>
#include <driverVideo.h>
#include <driverKeyboard.h>
#include <naiveConsole.h>

#define ROWS 25
#define COLS 80

static char KEYS_MAPPING[] = {0, 'ESC', '1', '2', '3', '4', '5', '6', '7', '8', '9', 			// 0 - 10
									'0', '\'', '¿', 'BACKSPACE', 'TAB', 'q', 'w', 'e', 'r', 't',// 11 - 20
								  	'y', 'u', 'i', 'o', 'p', '´', '+', '\n', 'MAYUS', 'a',		// 21 - 30
									's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0, '{', 			// 31 - 40
								  	'|', 'LSHIFT', '}', 'z', 'x', 'c', 'v', 'b', 'n', 'm',  	// 41 - 50
								  	',', '.', '-', 0, '*', 0, ' ', 0, 0, 0};					// 51 - 60										// 55 - 62

static char SHIFT_KEYS_MAPPING[] = {0, 'ESC', '!', '\0', '#', '$', '%', '&', '/', '(', ')', 	// 0 - 10
									'=', '?', '¡', 0, 'TAB', 'Q', 'W', 'E', 'R', 'T',			// 11 - 20
								  	'Y', 'U', 'I', 'O', 'P', '¨', '*', '\n', 'MAYUS', 'A',		// 21 - 30
									'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, '[', 			// 31 - 40
								  	'°', 'LSHIFT', ']', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',  	// 41 - 50
								  	';', ':', '_', 0, '*', 0, ' ', 0, 0, 0};					// 51 - 60

static int mayus = 0;
static int bufferPlace = 0;
static char buffer[ROWS*COLS] = {0};

void keyboard_handler(void) {
	int keycode;
	keycode = get_key();

	buffer[bufferPlace] = keycode;
	// set_nccursor(get_vdcursor());
	// ncPrintHex(keycode);
	// ncPrintChar(' ');
	// set_vdcursor(get_nccursor());

	if(keycode == 0x2A || keycode == 0x36 || (keycode == 0x3A && mayus == 0) ){ 
		mayus =!mayus;
		return;
	}
	if(keycode == 0xAA || keycode == 0xB6 || (keycode == 0x3A && mayus == 1) ){
		mayus =!mayus;
		return;
	}

	if(keycode >= 0 &&  keycode < MAX_KEYPRESSED) {

		switch(keycode) {
			case 14:
				// if(buffer[bufferPlace-1] == 28) {
				// 	backspace(1);
				// }
				backspace();
				bufferPlace--;
				break;
			case 15:
				printString("    ");
				bufferPlace++;
				break;
			case 28:
			case 224:
				newline();
				bufferPlace++;
				break;
			default:
				if(mayus) {
					printChar(SHIFT_KEYS_MAPPING[keycode]);
				} else {
					printChar(KEYS_MAPPING[keycode]);
				}
				bufferPlace++;
				break;
		}
	}
}