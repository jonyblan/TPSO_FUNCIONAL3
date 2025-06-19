#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <naiveConsole.h>

extern void saveRegisters();

unsigned int getKeyPressed();
void writeIntoBuffer();
unsigned char nextFromBuffer();

#endif