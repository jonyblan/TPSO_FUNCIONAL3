#include <soundDriver.h>
#include <stdint.h>
#include <lib.h>

static void play_sound(uint32_t nFrequence)
{
	uint32_t Div;
	uint8_t tmp;

	// Seteamos el canal 2 del PIT para que tickee a una frecuencia que podemos escuchar
	Div = 1193180 / nFrequence;
	outb(0x43, 0xb6);
	outb(0x42, (uint8_t)(Div));
	outb(0x42, (uint8_t)(Div >> 8));

	// "Activando" el bit 0 del puerto 61 linkeamos el canal 2 del PIT a los PC speakers, haciendo que cuando este "tickea"
	// los speakers se muevan a la posición "in", y cuando este vuelve a ticker, los speakers se mueven a la posición "out".
	tmp = inb(0x61);
	if (tmp != (tmp | 3))
	{
		outb(0x61, tmp | 3);
	}
}

//"Desactiva" el bit 0 del puerto 61, desvinculando el PIT de los speakers
static void nosound()
{
	uint8_t tmp = inb(0x61) & 0xFC;

	outb(0x61, tmp);
}

void timer_wait(int ticks)
{
	int now = ticks_elapsed();
	while (ticks_elapsed() - now < ticks)
		;
	return;
}

// Hace un beep
void beep(uint16_t hz, uint16_t ticks)
{
	if(hz != 1)
		play_sound(hz);
	timer_wait(ticks);
	nosound();
}