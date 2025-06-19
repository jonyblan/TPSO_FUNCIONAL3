#include <scheduler.h>
#include <stdint.h>
#include <stddef.h>
#include <PCBQueueADT.h>
#include <processManager.h>
#include <memoryManager.h>
#include <time_and_rtc.h>

schedulerStruct *scheduler;

void initScheduler(void *stackBase) {
  scheduler = malloc(sizeof(schedulerStruct));
  if(scheduler == NULL) {
    vdPrint("Error al inicializar el scheduler: malloc fallido");
    sleep(1);
    return;
  }
  for (uint8_t i = 0; i < MAX_PRIO; i++) {
    scheduler->schedule[i] = CreatePCBQueueADT();
  }

  scheduler->currentRunningPCB = malloc(sizeof(PCB));
  scheduler->currentRunningPCB->state= TERMINATED;
  scheduler->currentRunningPCB->stackBase = stackBase;
  for (uint8_t i = 0; i < MAX_PRIO; i++) {
      scheduler->count[i] = 0;
  }
}

void scheduleProcess(PCB *pcb) {
  queueProcess(scheduler->schedule[pcb->priority], pcb);
}

void descheduleProcess(PCB *pcb) {
  removeProcess(scheduler->schedule[pcb->priority], pcb);
}

void *schedule(void *rsp) {
    if (!scheduler || !scheduler->currentRunningPCB)
        return rsp;

    PCB *old = scheduler->currentRunningPCB;
    old->stackPointer = rsp;

    // Guardar estado del proceso actual
    if (old->state == RUNNING) {
        old->state = READY;
        queueProcess(scheduler->schedule[old->priority], old);
    }

    PCB *next = NULL;

    // Elegir el próximo proceso según presupuesto
    for (int8_t i = MAX_PRIO - 1; i >= 0; i--) {
        if (!isEmpty(scheduler->schedule[i]) && scheduler->count[i] <= (i * 5 + 1)) {
            next = dequeueProcess(scheduler->schedule[i]);
			/* if(next->state != READY){
				queueProcess(scheduler->schedule[next->priority], next);
				continue;
			} */
            scheduler->currentRunningPCB = next;
            scheduler->currentRunningPCB->state = RUNNING;
            scheduler->count[i]++;
            return next->stackPointer;
        }
    }

    // Si nadie fue elegido, reseteo y vuelvo a intentar
    for (int i = 0; i < MAX_PRIO; i++)
        scheduler->count[i] = 0;

    // Segunda pasada después del reset
    for (int8_t i = MAX_PRIO - 1; i >= 0; i--) {
        if (!isEmpty(scheduler->schedule[i])) {
            next = dequeueProcess(scheduler->schedule[i]);
            scheduler->currentRunningPCB = next;
            scheduler->currentRunningPCB->state = RUNNING;
            scheduler->count[i]++;
            return next->stackPointer;
        }
    }

    // No hay procesos listos
    return rsp;
}

uint16_t getCurrentPID(){return scheduler->currentRunningPCB->pid;}
PCB* getCurrentProcess(){return scheduler->currentRunningPCB;}