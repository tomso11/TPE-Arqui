#include <stdint.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <lib.h>

#include <driverVideo.h>
#include <interruptions.h>
#include <driverKeyboard.h>
#include <mouseDriver.h>
#include <shell.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const shellAddress = (void*)0x600000; // elijo una posicion de memoria que no voy a pisar
static void * const superUserAddress= (void*)0x700000;
static void * const currentAddress = (void*)0xA00000; // address logico donde compila nuestro modulo

typedef int (*EntryPoint)();
typedef int (*EntryPointS)(int);
typedef void (*handler_t)(void);

int choose_mod(char c);

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = { //cargamos los modulos en memoria 
		sampleCodeModuleAddress,
		sampleDataModuleAddress,
		shellAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

/* mapeo de los modulos a una direccion fisica para correr */
void mapModules(uint64_t  phys_addr ){
		uint64_t * PDbase= (uint64_t*) 0x10000; // base del page direc
		uint64_t * userEntry= PDbase + 4;
		*userEntry= phys_addr + 0x8F;// + 0x8B;
		return;

}

/* copia el modulo a ejecutar al address donde correremos todos los programas */
void copy_mod(uint64_t mod_addr){
	memcpy(currentAddress, mod_addr, 0x20000); // son 128KB, es arbitrario pero me parece que ningun modulo va a ocupar mas que eso
}

int main()
{	
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();
	ncNewline();
	

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	ncPrint("[Finished]");

	cli();

	
	iSetHandler(0x20, (uint64_t) irq0Handler);
	iSetHandler(0x21, (uint64_t) &irq1Handler);
	iSetHandler(0x2C, (uint64_t) &irq12Handler);
	iSetHandler(0x80, (uint64_t) sys_callHandler);
	
	setPicMaster(0xF9);
	setPicSlave(0x0);


	initialize_Mouse();
	initialize_cursor();

	sti();
	saveCR3();

	clear();

	flow_manager();

	clear();
	
	shell();
	while(1);

	return 0;
}

/* Da la opcion de inicializar cualquiera de los modulos,
	continuara corriendo permanentemente despues de ejecutar algun modulo */
void flow_manager(){
	char * option;
	int loop ;
	int mod;
	while(1){
		printString( " Welcome to gatOS, please input the module number you would like to run.\n        1- sampleCodeModule\n        2-Shell Module\n        3-SuperUser Module\n");
		loop=1;
		while( loop ){
			mod=choose_mod(poll_keyboard_buffer(option,1));
			switch(mod){
				case 1:
					printString("enter 1\n");
					copy_mod(sampleCodeModuleAddress);
					((EntryPoint)currentAddress)(); // supuestamente de esta manera corremos el modulo
					loop=0;
					break;
				case 2:
				printString("enter 2\n");
					copy_mod(shellAddress);
					//((EntryPoint)currentAddress)();
					shell();
					loop=0;
					break;
				case 3:
				printString("enter 3\n");
					copy_mod(superUserAddress);
					((EntryPoint)currentAddress)();
					loop=0;
					printString("gmh");
					break;
				default:
					break;
			}
		}
	}
}

int choose_mod(char c){
	if(c == '1')
		return 1;
	if(c== '2')
		return 2;
	if(c == '3')
		return 3;
	return 0;
}