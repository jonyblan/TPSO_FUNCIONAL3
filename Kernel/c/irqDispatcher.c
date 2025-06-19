#include <time_and_rtc.h>
#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

typedef void (*functionsArray[])();

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq)
{
	functionsArray functions = {int_20, int_21};
	functions[irq]();
	return;
}

void int_20()
{
	timer_handler();
}

void int_21()
{
	writeIntoBuffer();
}
