#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>

#define PIPE_BUFFER_SIZE 1024
#define MAX_PIPES 32
#define MAX_PIPE_NAME 32

typedef enum { PIPE_READ = 0, PIPE_WRITE = 1 } PipeEnd;

typedef struct {
    char name[MAX_PIPE_NAME];
    uint8_t inUse;

    char buffer[PIPE_BUFFER_SIZE];
    int readIdx;
    int writeIdx;
    int size;

    uint8_t read_sem;
    uint8_t write_sem;

} Pipe;

typedef struct {
    int inUse;
    Pipe* pipe;
    PipeEnd end;
} PipeFD;

// Abre (o crea si no existe) un pipe con nombre. Devuelve 0 si éxito, -1 si error.
uint8_t pipe_open(const char* name, int fds[2]);  // fds[0] = read end, fds[1] = write end

// Escribe `count` bytes del buffer `buf` al pipe indicado por el descriptor `fd`
uint64_t pipe_write(int fd, const char* buf, uint64_t count);

// Lee hasta `count` bytes del pipe indicado por el descriptor `fd` y los pone en `buf`
uint64_t pipe_read(int fd, char* buf, uint64_t count);

// Cierra un descriptor de pipe (read o write)
void pipe_close(int fd);

void initPipes();

#endif
