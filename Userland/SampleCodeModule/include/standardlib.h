#ifndef STANDARD_LIB_H
#define STANDARD_LIB_H

#include <stdint.h>

typedef uint64_t pid_t;

typedef enum
{
    STDOUT = 1,
    STDERR,
    STDMARK,
} FileDescriptor;

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint16_t year;
    uint8_t month;
    uint8_t day;
} TimeStamp;

void sleep(uint32_t ticks);
uint32_t unsigned_str_to_num(uint64_t *it, uint64_t buff_length, char *buff);
void unsigned_num_to_str(uint32_t num, uint32_t start, char *buff);
char *strchr(const char *str, int c);
TimeStamp* getTime();
uint64_t fdprintf(FileDescriptor fd, const char *fmt, ...);
uint64_t printf(const char *fmt, ...);
uint64_t scanf(const char *fmt, ...);
uint64_t readLine(char buff[], uint64_t count);
uint64_t putChar(uint64_t character);
uint8_t getChar();
uint8_t getc();
int strcmp(char *s1, char *s2);
void toMinus(char *str);
uint64_t getNextToRead(char *c);
void beep(uint32_t hz, uint32_t ticks);
void* malloc(uint64_t size);
void free(uint64_t* ptr);
int testMalloc();
pid_t createProcess(void* entryPoint, uint64_t argc, char *argv[]);
int getPriority(pid_t pid);
void setPriority(pid_t pid, int newPriority);
uint8_t sem_open(const char* name, uint8_t initial_value);
int sem_post(uint8_t id);
int sem_wait(uint8_t id);

uint8_t pipe_open(const char* name, int fds[2]);  // fds[0] = read end, fds[1] = write end

uint64_t pipe_write(int fd, const char* buf, uint64_t count);

uint64_t pipe_read(int fd, char* buf, uint64_t count);

void pipe_close(int fd);

void killProcess(pid_t pid);

void blockProcess(pid_t pid);

#endif