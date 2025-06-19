#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

uint8_t inb(uint8_t a);
void outb(uint8_t a, uint8_t b);

uint64_t strlen(const char *string);
int strcmp(const char* s1, const char* s2);
char* safe_strncpy(char *dest, const char *src, uint8_t max);
char *my_strcat(char *dest, const char *src);

#endif