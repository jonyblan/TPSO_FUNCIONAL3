#include <exceptions.h>
#include <syscallDispatcher.h>
#include <time.h>
#define ENTER 0x1C
#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6
#define REGISTERS 18

static void zero_division();
static void op_code();

void exceptionDispatcher(int exception)
{
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
	if (exception == OPCODE_EXCEPTION_ID)
		op_code();
}

static void returnToContext()
{
	uint64_t c = 0;
	while ((read(1, &c, 1) > 0))
		;
	while (c != '\n')
		read(1, &c, 1);
}

static void printRegisters()
{
	char *regName[] = {
		"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ",
		"RBP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ",
		"R13: ", "R14: ", "R15: ", "RSP: ", "RIP: ", "RFLAGS: "};
	vdPrint("Register Status: ");
	vdNewline();
	uint64_t *regsStatus = getRegs();
	uint64_t reg;
	for (int i = 0; i < REGISTERS; i++)
	{
		reg = regsStatus[i];
		vdPrint(regName[i]);
		vdPrintChar(' ');
		vdPrintHex(reg);
		vdNewline();
	}
}

static void exceptionMsg(char *msg)
{
	vdPrint("Exception: ");
	vdPrint(msg);
	vdNewline();
	printRegisters();
	sleep(50);
	vdPrint("Press ENTER to go back: ");
	returnToContext();
	vdClear();
}

static void zero_division()
{
	_sti();
	exceptionMsg("Cannot Divide By Zero");
}

static void op_code()
{
	_sti();
	exceptionMsg("Invalid Operation Code");
}
