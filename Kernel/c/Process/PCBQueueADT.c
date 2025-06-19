#include <stdint.h>
#include <memoryManager.h>
#include <PCBQueueADT.h>
#include <stddef.h>
#include <processManager.h>

#define MAX_PROCESSES 128

typedef struct PCBQueueCDT{
    PCB* queue[MAX_PROCESSES];
    uint8_t head;
    uint8_t tail;
    uint8_t size;
}PCBQueueCDT;

PCBQueueADT CreatePCBQueueADT(){
    PCBQueueADT toReturn = (PCBQueueADT) malloc(sizeof(*toReturn));
    if (toReturn != NULL)
    {  
        toReturn->head=0;
        toReturn->tail=0;
        toReturn->size=0;
    }
    return toReturn;
}

int queueProcess(PCBQueueADT adt, void* process){
    if (adt->size == MAX_PROCESSES) return -1;
    
    adt->size++;
    adt->queue[adt->tail]=process;
    adt->tail= (adt->tail + 1) % MAX_PROCESSES;
    return 0; 
}

void* dequeueProcess(PCBQueueADT adt){
    if (adt->size == 0) return NULL;

    void* toReturn =adt->queue[adt->head];
    adt->size--;
    adt->head= (adt->head+1) % MAX_PROCESSES;
    return toReturn;
}

int removeProcess(PCBQueueADT adt, PCB* PCB){
    if (adt->size == 0) return -1;

    uint8_t pos;
    uint8_t index = -1;
    for (uint8_t i = 0; i < adt->size; i++)
    {
        pos = (adt->head + i) % MAX_PROCESSES;
        if (adt->queue[pos]->pid == PCB->pid)
        {
            index= i;
            break;
        }
    }

    if (index == -1) return -1; // PCB not found
    
     for (int i = index; i < adt->size - 1; i++) {
        int from = (adt->head + i + 1) % MAX_PROCESSES;
        int to   = (adt->head + i) % MAX_PROCESSES;
        adt->queue[to] = adt->queue[from];
    }
    adt->tail = (adt->tail - 1 + MAX_PROCESSES) % MAX_PROCESSES;
    adt->size--;
    return 0; // PCB not found
}

uint8_t getPCBQueueSize(PCBQueueADT adt){return adt->size; }

uint8_t isEmpty(PCBQueueADT adt) { return adt->size <= 0; }