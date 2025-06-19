#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <stddef.h>
#include <PCBQueueADT.h>
#include <processManager.h>
#define MAX_PRIO 8


typedef struct schedulerStruct {
  PCBQueueADT schedule[MAX_PRIO];
  uint16_t count[MAX_PRIO];
  PCB* currentRunningPCB;
} schedulerStruct;

void initScheduler(void *stack_base);
void scheduleProcess(PCB *pcb);
void descheduleProcess(PCB *pcb);
void *schedule(void *rsp);
uint16_t getCurrentPID();
PCB* getCurrentProcess();

#endif
