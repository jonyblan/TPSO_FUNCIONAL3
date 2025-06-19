#include "buddyAllocator.h"

/* ---------- Estructuras internas -------------------------------- 
typedef struct FreeBlock {
    struct FreeBlock *next;
} FreeBlock;

/* 15 listas de bloques libres – una por orden 
static FreeBlock *free_lists[LEVELS] = {0};

#define ALIGN_UP(sz)  (((sz) + ((1U<<MIN_ORDER)-1)) & ~((1U<<MIN_ORDER)-1)) 
// Redondea hacia arriba al múltiplo de 64 B (alineación mínima).
// ej sz = 65 → ALIGN_UP(sz) = 128
// ej sz = 129 → ALIGN_UP(sz) = 192. el ~ 0 -> 1 ; 1 - > 0.


/* order mínimo tal que 2^(order+MIN_ORDER) ≥ size                
static inline int order_for_size(size_t size) {
    size_t s = 1U << MIN_ORDER;
    int order = 0;
    while (s < size) { s <<= 1; order++; }
    return order;
}


// calcula la direccion del buddy (gemelo) de un bloque,
// un par de buddies difiere exactamente en el bit cuyo peso es el tamaño del bloque.
// Ese bit esta en la posicion order + min_order. 
// XOR porque es mas eficiente que sumar o restar. si es par el buddy es mayor y al hacerle xor te devuelve al menor
// si es impar el buddy es menor y al hacerle xor te devuelve al mayor.
// Por ejemplo, si el bloque es de 64 B (order=0), su buddy es de 64 B y se encuentra a 64 B de distancia.
// Si el bloque es de 128 B (order=1), su buddy es de 128 B y se encuentra a 128 B de distancia.
static inline uintptr_t buddy_of(uintptr_t addr, int order) {
    return addr ^ (1UL << (order + MIN_ORDER));
}


/* ---------- Init ------------------------------------------------
void buddy_init(void) {
    for (int i = 0; i < LEVELS; i++) free_lists[i] = NULL;

    FreeBlock *first = (FreeBlock *)HEAP_START;
    first->next      = NULL;
    free_lists[LEVELS-1] = first;         /* bloque de 1 MiB 
}

/* ---------- Alloc ---------------------------------------------- 
void *buddy_alloc(size_t size) {
    if (!free_lists[LEVELS-1]) buddy_init();      /* 1ª vez 
    size = ALIGN_UP(size);
    int want = order_for_size(size);

    /* 1) Buscar primer nivel con bloque disponible 
    int i = want;
    while (i < LEVELS && !free_lists[i]) i++;
    if (i == LEVELS) return NULL;                 /* sin memoria 

    /* 2) Sacar bloque y hacer split mientras sea mayor que el pedido 
    FreeBlock *block = free_lists[i];
    free_lists[i]    = block->next;

    while (i > want) {
        i--;
        FreeBlock *buddy = (FreeBlock*)((uint8_t*)block + (1UL << (i+MIN_ORDER)));
        buddy->next      = free_lists[i];
        free_lists[i]    = buddy;
    }

    /* 3) Guardar el order en cabecera de 4 B y devolver 
    *((uint32_t*)block) = want;
    return (void*)((uint8_t*)block + sizeof(uint32_t));
}

/* ---------- Free ------------------------------------------------ 
void buddy_free(void *ptr) {
    if (!ptr) return;
    uint8_t *p   = (uint8_t*)ptr - sizeof(uint32_t);
    int order    = *((uint32_t*)p);
    uintptr_t addr = (uintptr_t)p;

    while (order < LEVELS-1) {
        uintptr_t buddy_addr = buddy_of(addr, order);

        /* Buscar al buddy en la free-list del mismo orden 
        FreeBlock **prev = &free_lists[order];
        FreeBlock  *curr = free_lists[order];
        while (curr && (uintptr_t)curr != buddy_addr) {
            prev = &curr->next;
            curr = curr->next;
        }
        if (!curr) break;                 /* buddy está ocupado 

        /*  -> fusionar: quitar buddy de lista y subir un nivel 
        *prev = curr->next;               /* lo saca de la lista 
        addr  = (addr < buddy_addr) ? addr : buddy_addr;
        order++;
    }

    /* Insertar bloque fusionado en su lista correspondiente 
    FreeBlock *blk = (FreeBlock*)addr;
    blk->next      = free_lists[order];
    free_lists[order] = blk;
}

/* ---------- Stats (para comando `mem`) -------------------------- 
void buddy_stats(char *buf, size_t n) {
    int len = 0;
    len += snprintf(buf+len, n-len, "Buddy Free-lists:\n");
    for (int i = 0; i < LEVELS; i++) {
        int count = 0;
        for (FreeBlock *b = free_lists[i]; b; b = b->next) count++;
        len += snprintf(buf+len, n-len,
                        "  %2d:  size %6u B  blocks %d\n",
                        i, 1U << (i+MIN_ORDER), count);
    }
}
    */
