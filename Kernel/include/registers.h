#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>
#include <stdio.h>

void makeBackup(uint64_t* regs);
char isBackupDone();
uint64_t* getRegs();

#endif