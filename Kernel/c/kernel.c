#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <idtLoader.h>
#include <processManager.h>
#include <videoDriver.h>
#include <scheduler.h>
#include <time_and_rtc.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

void testProcessA(int x) {
	while (1) {
		vdPrintDec(x);
		vdPrintChar('\n');
		sleep(10);
	}
}

void testProcessB() {
	while (1) {
		vdPrint("Process B running");
		vdPrintChar('\n');
		sleep(10);
	}
}
void testProcessC() {
	while (1) {
		vdPrint("Process C running");
		vdPrintChar('\n');
		sleep(10);
	}
}

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static int veces=0;


typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 // The size of the stack itself, 32KiB
					- sizeof(uint64_t)					  // Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{
	char buffer[10];

	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{	
	
	load_idt();
	
	initScheduler(getStackBase());

	char *argv[] = {0};
	createFirstProcess((void*)sampleCodeModuleAddress, 0, argv);
	setTickFrequency(120);
	_sti();
	while (1){}
	
	return 0;
}