#include <interruptions.h>
#include <driverVideo.h>
#include <driverKeyboard.h>
#include <mouseDriver.h>
#include <naiveConsole.h>
#include <systemCalls.h>


static int i = 0;
char *video = (char *) 0xB8000;

#pragma pack(push)
#pragma pack(1)

typedef struct { 
	uint16_t offset_l; //bit 0..15
	uint16_t selector;
	uint8_t zero_l;
	uint8_t attrs;
	uint16_t offset_m; //16..31
	uint32_t offset_h; //32..63
	uint32_t zero_h;
} IDTEntry_t;

#pragma pack(pop)


static IDTEntry_t* IDT = (IDTEntry_t*) 0x0;


void iSetHandler(int index, uint64_t handler) {
	IDT[index].offset_l = (uint16_t) handler & 0xFFFF;
	IDT[index].offset_m = (uint16_t) (handler >> 16) & 0xFFFF;
	IDT[index].offset_h = (uint32_t) (handler >> 32) & 0xFFFFFFFF;
	
	IDT[index].selector = 0x08;
	IDT[index].zero_l = 0;
	
	IDT[index].attrs = 0x8E;
	IDT[index].zero_h = 0;	
	
}

typedef void (*handler_t) (void);

// static int ticks=0;

void tickHandler() {
	//tick_handler();
	printString("ticks:");
	printString("\n");
	/* proof that timertick works wonders
	ticks++;
	printString("ticks:");
	ncPrintDec(ticks);
	printString("\n");
	*/

}

void next_process2(){
	printf("funcion nextprocess2\n");
	return;
};

void timer_handler2() {
	printf("1\n");
	/*sleep_process * current = sleeping_processes;
	sleep_process * prev = NULL;
	sleep_process * sp;
	printf("2\n");
	while (current != NULL) {
		current->ticks -= 1;
		printf("11\n");
		if (current->ticks <= 0) {
			sp = current;

			if (prev == NULL)
				sleeping_processes = current->next;
			else
				prev->next = current->next;

			unblock_process(current->p);
			current = current->next;
			store_page((uint64_t) sp);
		}
		else {
			prev = current;
			current = current->next;
		}
	}*/
	printf("3\n");
	return;
}

/* en "driverKeyboard.c" */
void keyboardHandler(){
	keyboard_handler(); 
}

/*en "mouseDriver.c"*/
void mouseHandler(){
	//testfoo();
	mouse_handle();

}

handler_t handlers[] = {tickHandler, keyboardHandler, mouseHandler, sys_callHandler}; // vector con todas las interrupciones que vamos a usar

void irqDispatcher(int irq){ //le pasamos el numero de interrupcion y nos lleva al handler adecuado
	handlers[irq]();
}