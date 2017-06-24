#include <stdint.h>
#include <moduleLoader.h>
#include <naiveConsole.h>

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
static void * const shellAddress = (void*)0xC00000; // elijo una posicion de memoria que no voy a pisar
static void * const currentAddress = (void*)0x300000; // puesto en shell.ld

typedef int (*EntryPoint)();
typedef int (*EntryPointS)(int);
typedef void (*handler_t)(void);


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
	void * moduleAddresses[] = {
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

void mapModules(uint64_t  phys_addr ){
		uint64_t * PDbase= (uint64_t*) 0x10000; // base del page direc
		uint64_t * userEntry= PDbase + 4;
		*userEntry= phys_addr + 0x8F;// + 0x8B;
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

	
	iSetHandler(0x20, (uint64_t) irq0Handler);
	iSetHandler(0x21, (uint64_t) &irq1Handler);
	iSetHandler(0x2C, (uint64_t) &irq12Handler);
	iSetHandler(0x80, (uint64_t) sys_callHandler);
	
	setPicMaster(0xF9);
	setPicSlave(0x0);


	initialize_Mouse();
	initialize_cursor();

	sti();
	//clear();
	mapModules(shellAddress);
	saveCR3();

	printString("At least you tried");


	(((EntryPoint)currentAddress)());

	clear();
	
	shell();
	while(1);

	return 0;
}
