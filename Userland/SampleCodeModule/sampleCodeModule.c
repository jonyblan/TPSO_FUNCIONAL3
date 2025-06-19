/* sampleCodeModule.c */
#include <standardlib.h>
#include <nanoshell.h>

char *v = (char *)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

extern void call_80h();

int main()
{
	startNanoShell();
	return 0xDEADBEEF;
}