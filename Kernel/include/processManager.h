#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#define PROCESS_MAX_NAME_LEN 40
#define PROCESS_STACK_SIZE 16384 // 16KiB
#define PRIORITY_LEVELS 8
#define DEFAULT_PRIORITY 2
#define MAX_PROCESSES 256

#include "../include/memoryManager.h"

typedef enum{
    READY = 0,
	RUNNING,
	BLOCKED,
	TERMINATED 
} State;

typedef struct ProcessControlBlock {
    pid_t pid;
    char name[PROCESS_MAX_NAME_LEN];        // Process name (optional but useful)
    
    State state;                  	// e.g., READY, RUNNING, BLOCKED, TERMINATED
    uint8_t priority;               // For scheduling
    
    void* stackBase;                // Base of the allocated stack
    void* stackPointer;             // Current stack pointer (for context switching)
    
    void* entryPoint;               // The function to run (e.g., main of the process)
    
    struct PCB* parent;         // For wait() logic
    struct PCB* next;           // For scheduling queue

    uint8_t fd[2];
    
    uint8_t foreground;                 // 1 = foreground, 0 = background
    uint8_t waitingChildren;            // Track if wait() is needed. 1 = wait() needed, 0 = wait() not needed
    
	uint8_t argc;
	char** argv;
}PCB;

typedef struct processStack{
    void *r15;
    void *r14;
    void *r13;
    void *r12;
    void *r11;
    void *r10;
    void *r9;
    void *r8;
    void *rsi;
    void *rdi;
    void *rbp;
    void *rdx;
    void *rcx;
    void *rbx;
    void *rax;
    void *rip;
    void *cs;
    void *rflags;
    void *rsp;
    void *ss;
}processStack;

extern PCB* currentProcess;

pid_t createProcess(void (*fn)(uint8_t, char **), int priority, int argc, char** argv);

int initializeProcesses();

void terminateProcess();

int killProcess(uint8_t pid);

void quantumTick(); //funcion para manejar los quantums

void showRunningProcesses();

void createFirstProcess(void (*fn)(uint8_t, char **), int argc, char** argv);

void setPriority(pid_t pid, int newPriority);

int getPriority(pid_t pid);

void yield();
#endif