#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H



#define MAX_PID 32768
#include <stdint.h>

typedef uint64_t pid_t;

void* malloc(uint64_t size);
void free(void* ptr);

#endif