#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>
#include <stddef.h>
#include "memoryManager.h"

#define HEAP_START 0x1000000
#define HEAP_SIZE  0x100000  // 1 MiB
#define BLOCK_SIZE 64
#define BLOCK_COUNT (HEAP_SIZE / BLOCK_SIZE)


typedef struct Block {
    size_t size;
    struct Block* next;
    int free;
} Block;


Block* head = (Block*)HEAP_START;

void init_heap() {
    head->size = HEAP_SIZE - sizeof(Block);
    head->next = NULL;
    head->free = 1;
}

uint8_t heapStarted = 0;

void* malloc(size_t size) {
    if (!heapStarted) {
        init_heap();
        heapStarted = 1;
    }

    Block* current = head;
    while (current != NULL) {
        if (current->free && current->size >= size) {
            // Optionally split block if too large
            if (current->size > size + sizeof(Block)) {
                Block* new_block = (Block*)((char*)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->next = current->next;
                new_block->free = 1;

                current->next = new_block;
                current->size = size;
            }

            current->free = 0;  // mark block as used
            return (void*)(current + 1);  // return the memory after the Block header
        }

        current = current->next;
    }

    return NULL;  // No free block large enough
}



void free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)ptr - 1;
    block->free = 1;

    // Optional: merge with next block
    if (block->next && block->next->free) {
        block->size += sizeof(Block) + block->next->size;
        block->next = block->next->next;
    }
}