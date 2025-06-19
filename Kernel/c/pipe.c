#include <pipe.h>
#include <lib.h>
#include <memoryManager.h>
#include <processManager.h>
#include <scheduler.h>
#include <PCBQueueADT.h>
#include <mySem.h>

static Pipe pipes[MAX_PIPES];
static PipeFD pipeFDs[MAX_PIPES * 2];  // cada pipe puede tener hasta 2 descriptores

static int started = 0;

void initPipes() {
    for (int i = 0; i < MAX_PIPES; i++) {
        pipes[i].inUse = 0;
    }
    for (int i = 0; i < MAX_PIPES * 2; i++) {
        pipeFDs[i].inUse = 0;
    }
}

static int allocateFD(Pipe* pipe, PipeEnd end) {
    for (int i = 0; i < MAX_PIPES * 2; i++) {
        if (!pipeFDs[i].inUse) {
            pipeFDs[i].inUse = 1;
            pipeFDs[i].pipe = pipe;
            pipeFDs[i].end = end;
            return i;
        }
    }
    return -1;
}

uint8_t pipe_open(const char* name, int fds[2]) {
	if (!started) {
		initPipes();
		started = 1;
	}

	Pipe* pipe = NULL;
	int i;

	// Ver si ya existe
	for (i = 0; i < MAX_PIPES; i++) {
		if (pipes[i].inUse && strcmp(pipes[i].name, name) == 0) {
			pipe = &pipes[i];
			break;
		}
	}
    
	// Si no existe, crearlo
	if (!pipe) {
        for (i = 0; i < MAX_PIPES; i++) {
            if (!pipes[i].inUse) {
                pipe = &pipes[i];
				pipe->inUse = 1;
				safe_strncpy(pipe->name, name, MAX_PIPE_NAME);
				pipe->name[MAX_PIPE_NAME - 1] = '\0';  // Asegurar nul-terminación
				pipe->readIdx = 0;
				pipe->writeIdx = 0;
				pipe->size = 0;
               	pipe->write_sem=sem_open(my_strcat(name,"read"), PIPE_BUFFER_SIZE-1);
                pipe->read_sem=sem_open(name, 0);
				break;
			}
		}
	}

	if (!pipe) return -1;  // No hay espacio

	// Asignar file descriptors
	int readFD = allocateFD(pipe, PIPE_READ);
	int writeFD = allocateFD(pipe, PIPE_WRITE);

	if (readFD == -1 || writeFD == -1) return -1;

	fds[0] = readFD;
	fds[1] = writeFD;

	return i;
}

uint64_t pipe_write(int fd, const char* buf, uint64_t count) {
    if (fd < 0 || fd >= MAX_PIPES * 2 || !pipeFDs[fd].inUse || pipeFDs[fd].end != PIPE_WRITE)
        return -1;

    Pipe* pipe = pipeFDs[fd].pipe;
    int written = 0;

    while (written < count) {
        sem_wait(pipe->write_sem);

        pipe->buffer[pipe->writeIdx] = buf[written];
        pipe->writeIdx = (pipe->writeIdx + 1) % PIPE_BUFFER_SIZE;
        pipe->size++;
        written++;
        sem_post(pipe->read_sem);
    }

    return written;
}

uint64_t pipe_read(int fd, char* buf, uint64_t count) {
    if (fd < 0 || fd >= MAX_PIPES * 2 || !pipeFDs[fd].inUse || pipeFDs[fd].end != PIPE_READ)
        return -1;

    Pipe* pipe = pipeFDs[fd].pipe;
    int read = 0;

    while (read < count) {
        sem_wait(pipe->read_sem);
        buf[read] = pipe->buffer[pipe->readIdx];
        pipe->readIdx = (pipe->readIdx + 1) % PIPE_BUFFER_SIZE;
        pipe->size--;
        read++;
        sem_post(pipe->write_sem);
    }

    return read;
}

void pipe_close(int fd) {
    if (fd < 0 || fd >= MAX_PIPES * 2 || !pipeFDs[fd].inUse)
        return;

    pipeFDs[fd].inUse = 0;
}

void resetBuffer(uint16_t fd){
    for (size_t i = 0; i < PIPE_BUFFER_SIZE; i++)
    {
        pipeFDs[fd].pipe->buffer[i] = 0;
    }
}