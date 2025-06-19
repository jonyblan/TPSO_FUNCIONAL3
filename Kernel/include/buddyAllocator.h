#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

/* === Parámetros del heap ======================================== */
#define HEAP_START   0x01000000          /* 16 MiB  (fuera de Kernel+Modules) */
#define HEAP_ORDER   20                  /* 2^20 = 1 MiB tamaño total          */
#define MIN_ORDER     6                  /* 2^6  =   64 B bloque mínimo        */
#define LEVELS       (HEAP_ORDER - MIN_ORDER + 1)

/* === API pública (idéntica a malloc/free) ======================== */
void  *buddy_alloc (size_t size);
void   buddy_free  (void *ptr);
void   buddy_init  (void);
void   buddy_stats(char *buf, size_t n); /* imprime free-lists p/ comando mem */

#endif