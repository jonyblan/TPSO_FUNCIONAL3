#ifndef _SOUND_DRIVER_H
#define _SOUND_DRIVER_H

#include <stdint.h>


void beep(uint16_t hz, uint16_t ticks);
void timer_wait(int ticks);

#endif