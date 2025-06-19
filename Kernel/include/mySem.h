#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "PCBQueueADT.h"

#define MAX_SEMAPHORES 32
#define MAX_NAME_LEN 32

void _cli(void);
void _sti(void);

typedef struct {
    int value;
    PCBQueueADT waiters;
} Semaphore;

typedef struct {
    int inUse;
    char name[MAX_NAME_LEN];
    Semaphore sem;
} NamedSemaphore;

uint8_t sem_init(Semaphore* sem, uint8_t initial_value);
uint8_t sem_open(const char* name, uint8_t initial_value);
int sem_wait(uint8_t id);
int sem_post(uint8_t id);
void sem_destroy(uint8_t id);

#endif