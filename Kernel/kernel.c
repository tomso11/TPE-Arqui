

#include <stdint.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <lib.h>

#include <driverVideo.h>
#include <interruptions.h>
#include <driverKeyboard.h>
#include <mouseDriver.h>
#include <shell.h>

#define MAX_CONTEXTS 12

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
static void * const superUserAddress= (void*)0x800000;
static void * const currentAddress = (void*)0x700000; // address logico donde compila nuestro modulo
static void * const dumbModuleAddress = (void*)0xA00000;

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
		sampleDataModuleAddress,shellAddress,superUserAddress,
		dumbModuleAddress

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


//Copio el modulo a la direccion de memoria donde se correra el codigo
void copy_mod(uint64_t mod_addr){
	memcpy((void*)currentAddress, (void*)mod_addr, PageSize);
}

 /* mapeo de los modulos a una direccion fisica para correr */
void mapModules(uint64_t  phys_addr ){
 		uint64_t * PDbase= (uint64_t*) 0x10000; // base del page direc
 		uint64_t * userEntry= PDbase + 4;
 		*userEntry= phys_addr+0x8F;// + 0x8B;
 		return;

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

	/* Seteamos los handlers apropiados para cada interrupcion*/
	iSetHandler(0x20, (uint64_t) irq0Handler);
	iSetHandler(0x21, (uint64_t) &irq1Handler);
	iSetHandler(0x2C, (uint64_t) &irq12Handler);
	iSetHandler(0x80, (uint64_t) sys_callHandler);
	
	setPicMaster(0xF9);
	setPicSlave(0x0);


	initialize_Mouse();
	initialize_cursor();
	printChar('\n');


	sti();
	//page_enable();
	saveCR3();
	//clear();
	ncPrintHex(&endOfKernel);
	printChar('\n');
	ncPrintHex(&endOfKernelBinary);
	printChar('\n');
	ncPrintHex(&bss);
	printChar('\n');

	//initializeKernelBinary();
	clear();
	flow_manager();

	//clear();
	
	//shell();
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
		printString( "Welcome to gatOS, please input the module number you would like to run.\n        1- SampleCode Module\n        2-Shell Module\n        3-SuperUser Module\n        4-Dumb Module\n");
		loop=1;
		while( loop ){
			mod=choose_mod(poll_keyboard_buffer(option,1));
			switch(mod){
				case 1:
					//printString("enter 1\n");
					copy_mod((uint64_t)sampleCodeModuleAddress);
					mapModules((uint64_t) currentAddress);
					//saveCR3();
					//((EntryPoint)sampleCodeModuleAddress)(); 
					((EntryPoint)currentAddress)();
					loop=0;
					break;
				case 2:
					//printString("enter 2\n");
					copy_mod((uint64_t)shellAddress);
					mapModules((uint64_t)currentAddress);
					//saveCR3();
					//ncPrintHex(shellAddress);
					//printChar('\n');
					//ncPrintHex(currentAddress);
					//printChar('\n');
					//run_mod(shellAddress);
					((EntryPoint)currentAddress)();
					//((EntryPoint)shellAddress)();
					//shell();
					loop=0;
					break;
				case 3:
					printString("enter 3\n");
					copy_mod((uint64_t)superUserAddress);
					mapModules((uint64_t)currentAddress);
					//saveCR3();
					((EntryPoint)currentAddress)();
					//copy_mod(superUserAddress);
					//((EntryPoint)superUserAddress)();
					//superUser();
					loop=0;
					break;
				case 4:
					//printString("enter 4\n");
					copy_mod((uint64_t)dumbModuleAddress);
					mapModules((uint64_t)currentAddress);
					//((EntryPoint)dumbModuleAddress)();
					((EntryPoint)currentAddress)();
					loop=0;
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
	if(c== '4')
		return 4;
	return 0;
}


// /* mapeo de los modulos a una direccion fisica para correr */
// void mapModules(uint64_t  phys_addr ){
// 		uint64_t * PDbase= (uint64_t*) 0x10000; // base del page direc
// 		uint64_t * userEntry= PDbase + 4;
// 		*userEntry= phys_addr + 0x8F;// + 0x8B;
// 		return;

// }

// /* copia el modulo a ejecutar al address donde correremos todos los programas */
// void copy_mod(uint64_t mod_addr, int prev_context ){
// 	uint64_t phys_addr;	
// 	n_context++;
// 	error=add_context(n_context, prev_context);
// 	if ( error ){
// 			printString("Max amount of context created, please restart.\n");
// 			n_context--;
// 			return;
// 	}
// 	phys_addr=contexts[n_context].phys;	
// 	memcpy(phys_addr, mod_addr, PageSize); // son 128KB, es arbitrario pero me parece que ningun modulo va a ocupar mas que eso
		
// }	

// uint64_t get_context_phys(int n_context){
// 	return currentAddress+(PageSize*n_context);
// }

// int add_context(int n_context, int prev_context){
// 	if(n_context > 11){
// 		return 1;
// 	}
// 	uint64_t phys_addr= get_context_phys(n_context);
// 	contexts[n_context].prev_context=prev_context;
// 	contexts[n_context].phys=phys_addr;	
// }

// /* syscall run handler*/
// void loader(int option){
// 	parseOption(option);
// 	copy_mod(option, current_context);
// 	current_context=n_context;
// 	((EntryPoint)option)();
// //	current_context=contexts[n_context].prev_context;   //esto solo es necesario si no vuelve al contexto anterior de forma automatica
// //	((EntryPoint)prev_context)();
	
// }