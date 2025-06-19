#include <processManager.h>
#include <stdint.h>
#include <stddef.h>
#include <memoryManager.h>
#include <PCBQueueADT.h>
#include <videoDriver.h>
#include <scheduler.h>
#include <lib.h>

extern void idle();

PCB processes[MAX_PROCESSES];
static PCBQueueADT terminatedProcessesQueue; //Cola de procesos esperando a que le hagan wait() (Si terminan, su PCB se marca como TERMINATED, por lo que se podria pisar el PCB)
PCB* currentProcess;
static uint8_t processCount= 1;
static pid_t nextPID= 1;

int getPriority(pid_t pid){
	if(pid < 0 || pid > MAX_PROCESSES){
		return -1;
	}
	return processes[pid].priority;
}

void setPriority(pid_t pid, int newPriority){
	if(pid < 0 || pid > MAX_PROCESSES){
		return ;
	}
	processes[pid].priority = (newPriority >= PRIORITY_LEVELS) ? PRIORITY_LEVELS - 1 : newPriority;
}

void myExit(){
    uint16_t pid = getCurrentPID();
    killProcess(pid);
}

void launchProcess(void (*fn)(uint8_t, char **), uint8_t argc, char *argv[]) {
  fn(argc, argv);
  myExit();
}


void *stackStart= (void*) 0x1000000;



void prepareStack(PCB* PCB, void* stack,void* entrypoint) {
    processStack *pStack = stack - sizeof(processStack);
    pStack->rsp = stack;
    pStack->rbp = stack;
    pStack->cs = (void *)0x8;
    pStack->rflags = (void *)0x202;
    pStack->ss = 0x0;
    pStack->rip = entrypoint;
    PCB->stackPointer = pStack;
}

void loadArguments(void (*fn)(uint8_t, char **), uint8_t argc, char *argv[],
                    void *stack) {
    processStack *pStack = stack - sizeof(processStack);
    pStack->rdi = fn;
    pStack->rsi = (void *)(uintptr_t)argc; 
    pStack->rdx = argv; 
}
void createFirstProcess(void (*fn)(uint8_t, char **), int argc, char** argv){
    PCB* new= &processes[0];
    new->pid=0;
    new->state=READY;
    new->priority=1;
    //new->stackBase= malloc(PROCESS_STACK_SIZE);
    new->stackBase = stackStart;
    new->entryPoint=launchProcess;
    processStack *newStack = new->stackBase - sizeof(processStack);
    newStack->rsp = new->stackBase;
    newStack->rbp = new->stackBase;
    newStack->cs = (void *)0x8; // Kernel code segment
    newStack->rflags = (void *)0x202; // Set interrupt flag
    newStack->ss = 0x0; // Kernel data segment
    newStack->rip = launchProcess; // Entry point for the idle process
    newStack->rdi = fn;
    newStack->rsi = (void *)(uintptr_t) argc; // Argument count
    newStack->rdx = argv; // Argument vector
    new->stackPointer = newStack;
    scheduleProcess(new);
    
    processCount++;
}

pid_t createProcess(void (*fn)(uint8_t, char **), int priority, int argc, char** argv){
    if (processCount>= MAX_PROCESSES) return -1;
    PCB* new=NULL;
    int pid= nextPID++;
    
    new = &processes[pid];
    new->pid = pid;
    new->priority = (priority >= PRIORITY_LEVELS) ? PRIORITY_LEVELS - 1 : priority;
    new->foreground = 1;
    new->waitingChildren = 0;
    new->argc = argc;
    new->argv = argv;
    new->entryPoint = launchProcess;
    new->parent = &processes[getCurrentPID()];
    new->next = NULL;

    char **args = malloc(sizeof(char*) * (argc+1));
    for (uint8_t i = 0; i < argc; i++)
    {
        uint64_t len = strlen(argv[i]) + 1;
        args[i] = malloc(len);
        memcpy(args[i], argv[i], len);  
    }
    args[argc] = NULL; 
    new->argv = args;
    //new->stackBase=malloc(PROCESS_STACK_SIZE);
    new->stackBase = (stackStart + new->pid * PROCESS_STACK_SIZE);


    
    prepareStack(new, new->stackBase, new->entryPoint);
    loadArguments(fn, argc, argv,new->stackBase);
    new->state = READY;
    scheduleProcess(new);
    
    processCount++;
    
    return new->pid;
}

int blockProcess(uint16_t pid) {
    if (pid >= MAX_PROCESSES) {
        return -1; // Invalid PID or process already terminated
    }
    processes[pid].state = BLOCKED;
    if(getCurrentPID() == pid) {
        yield(); // If the current process is blocked, yield to allow other processes to run
    }
    else {
        // If not the current process, just change state
        descheduleProcess(&processes[pid]);
    }
    return 0;
}

int unblockProcess(uint16_t pid) {
    if (pid >= MAX_PROCESSES) {
        return -1; // Invalid PID or process already terminated
    }
    if (processes[pid].state == BLOCKED) {
        processes[pid].state = READY;
        scheduleProcess(&processes[pid]);
    }
    return 0; // Process unblocked successfully
}

/* PCB* getNextProcess() {
    for (int priority = 0; priority < PRIORITY_LEVELS; priority++) {
        PCBQueueADT queue = processQueues[priority];
        int queueSize = getPCBQueueSize(queue);  

        for (int i = 0; i < queueSize; i++) {
            PCB* candidate = dequeueProcess(queue);  
            if (candidate->state == READY ) {
                queueProcess(queue, candidate);     
                return candidate;
            } else {
                queueProcess(queue, candidate);     
            }
        }
    }
    return &processes[0];
} */


int killProcess(uint8_t pid){
    for (size_t i = 0; i < MAX_PROCESSES; i++)
    {
        if (processes[i].pid == pid)
        {
            if (processes[i].state == TERMINATED) return -1;
            
            processes[i].state = TERMINATED;
            queueProcess(terminatedProcessesQueue,&processes[i]);
            processCount--;
            if (getCurrentPID() == pid)
            {
                yield();
            }
            return 0;
        }
    }
    return -1;
}

void yield(){return;}

void showRunningProcesses(){
    for (size_t i = 0; i < MAX_PROCESSES ; i++)
    {
        if (processes[i].state== READY || processes[i].state== RUNNING)
        {
            vdPrint("Process "); vdPrintDec(i); vdPrint(" Is running"); vdPrintChar('\n');
        }
        
    }
    return;
}

void cleanTerminatedList(){
    uint8_t size= getPCBQueueSize(terminatedProcessesQueue);
    PCB* current;
    for (uint8_t i = 0; i < size; i++)
    {
        current= dequeueProcess(terminatedProcessesQueue);
        if (current->waitingChildren)
        {
            queueProcess(terminatedProcessesQueue,current);
        }
        else
        {
            free(current->stackBase);
        }
    }
}
/* #include <processManager.h>
#include <stddef.h>
#include <videoDriver.h>
#include <syscallDispatcher.h>

// This would actually "start" the process by switching context to it
extern void loadProcessAsm(Process* process);

static void terminateProcess() {
    // Loop forever for now, since there's no scheduler to switch away
    while (1) {}
}

// For now, we just support one process
static Process* firstProcess = NULL;

void enqueueProcess(Process* process) {
    firstProcess = process;
}

Process* allocateProcessStruct(uint8_t argc) {
    // Allocate space for Process + argv array
    Process* newProcess = (Process*)malloc(sizeof(Process) + sizeof(char*) * argc);
    return newProcess;
}

pid_t generatePid() {
    return 1; // Basic version that always returns 1
}

char* strcpy(char* dest, const char* src) {
    char* original = dest;
    while ((*dest++ = *src++) != '\0');
    return original;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void* setupStack(Process* p) {
    uint64_t* stack = (uint64_t*)((uint8_t*)p->stackBase + PROCESS_STACK_SIZE);

    // Stack grows downward: push values in reverse call order

    // Push fake return address (if the function returns, go to terminateProcess)
    *(--stack) = (uint64_t)terminateProcess;

    // Push arguments as expected by your calling convention
    *(--stack) = (uint64_t)p->argc;
    *(--stack) = (uint64_t)p->argv;

    // Push the "return address" for the entryPoint, simulating a call
    *(--stack) = (uint64_t)p->entryPoint;

    // Push base pointer (fake rbp)
    *(--stack) = 0;

    return (void*)stack;
}

pid_t create(void* entryPoint, uint8_t priority, uint8_t foreground, uint8_t argc, char* argv[]) {
    Process* newProcess = allocateProcessStruct(argc);
    newProcess->pid = generatePid();

    // Copy argv pointers (strings can stay in caller's memory for now)
    for (int i = 0; i < argc; i++) {
    	size_t len = strlen(argv[i]) + 1;
    	newProcess->argv[i] = malloc(len);
    	strcpy(newProcess->argv[i], argv[i]);
	}

	newProcess->argc = argc;

    newProcess->entryPoint = entryPoint;
    newProcess->priority = priority;
    newProcess->foreground = foreground;
    newProcess->state = READY;

    strcpy(newProcess->name, argv[0]);

    newProcess->stackBase = malloc(PROCESS_STACK_SIZE);
    newProcess->stackPointer = setupStack(newProcess);

    enqueueProcess(newProcess);
    return newProcess->pid;
}

pid_t createProcess(void* entryPoint) {
	int argc = 2;
	char* argv[] = {"simpleProcess\0", NULL};
    pid_t pid = create(entryPoint, DEFAULT_PRIORITY, 1, argc, argv);
    loadProcessAsm(firstProcess);
    return pid;
}
 */