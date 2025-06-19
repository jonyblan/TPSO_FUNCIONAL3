#include <registers.h>

#define REGISTERS 18

uint64_t registerBackup[REGISTERS];
char backupDone;

void makeBackup(uint64_t *regs)
{
    for (int i = 0; i < REGISTERS; i++)
    {
        registerBackup[i] = regs[i];
    }

    backupDone = 1;
    return;
}

char isBackupDone()
{
    return backupDone == 1;
}

uint64_t *getRegs()
{
    if (isBackupDone())
        return registerBackup;
    return NULL;
}
