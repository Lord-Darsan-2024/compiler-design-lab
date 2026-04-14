/*
 * ============================================================================
 * Lab 15: Heap Storage Allocation – First-Fit Strategy
 * ============================================================================
 *
 * DESCRIPTION:
 * This laboratory simulates dynamic heap memory allocation using the first-fit
 * strategy with explicit free list management. The program maintains a free
 * list of available blocks and allocated blocks. On malloc, the first fitting
 * block is allocated; on free, the block is returned to the free list with
 * coalescing of adjacent free blocks when appropriate. The simulator displays
 * heap state after each operation.
 *
 * INPUT:
 * Sequence of malloc/free operations (6+)
 *
 * OUTPUT:
 * Heap layout table showing block address, size, and status
 *
 * COMPILATION AND EXECUTION:
 * gcc -std=c99 -Wall -Wextra -O2 -o storage_allocation storage_allocation.c
 * ./storage_allocation
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HEAP_SIZE 1024

typedef struct {
    int address;
    int size;
    bool allocated;
    char name[20];  /* Variable name or "Free" */
} Block;

typedef struct {
    Block blocks[50];
    int block_count;
} Heap;

void display_heap(Heap *heap, int step) {
    printf("\n%-3d | ", step);
    printf("%-30s | ", "Heap Layout");
    printf("%-20s\n", "Status");
    printf("----|--------------------------------|-----------\n");

    for (int i = 0; i < heap->block_count; i++) {
        Block *b = &heap->blocks[i];
        printf("----|");
        printf(" Addr:%4d Size:%3d %s |", b->address, b->size,
               b->allocated ? "[ALLOC]" : "[FREE] ");
        printf(" %-20s\n", b->name);
    }
}

void malloc_sim(Heap *heap, int size, const char *name) {
    printf("\nmalloc(%d, \"%s\")\n", size, name);

    for (int i = 0; i < heap->block_count; i++) {
        if (!heap->blocks[i].allocated && heap->blocks[i].size >= size) {
            /* Found fitting block */
            Block allocated = heap->blocks[i];
            allocated.allocated = true;
            strcpy(allocated.name, name);
            allocated.size = size;

            /* If block is larger, split */
            if (heap->blocks[i].size > size) {
                Block free_block;
                free_block.address = allocated.address + size;
                free_block.size = heap->blocks[i].size - size;
                free_block.allocated = false;
                strcpy(free_block.name, "Free");

                /* Insert new free block */
                for (int j = heap->block_count; j > i + 1; j--) {
                    heap->blocks[j] = heap->blocks[j-1];
                }
                heap->blocks[i+1] = free_block;
                heap->block_count++;
            }

            heap->blocks[i] = allocated;
            return;
        }
    }

    printf("ERROR: No fitting block found!\n");
}

void free_sim(Heap *heap, const char *name) {
    printf("\nfree(\"%s\")\n", name);

    for (int i = 0; i < heap->block_count; i++) {
        if (heap->blocks[i].allocated && strcmp(heap->blocks[i].name, name) == 0) {
            heap->blocks[i].allocated = false;
            strcpy(heap->blocks[i].name, "Free");

            /* Coalesce with next free block */
            if (i + 1 < heap->block_count && !heap->blocks[i+1].allocated) {
                heap->blocks[i].size += heap->blocks[i+1].size;
                for (int j = i + 1; j < heap->block_count - 1; j++) {
                    heap->blocks[j] = heap->blocks[j+1];
                }
                heap->block_count--;
            }

            return;
        }
    }

    printf("ERROR: Block not found!\n");
}

int main(void) {
    printf("\n");
    printf("========================================================================\n");
    printf("        HEAP STORAGE ALLOCATION – FIRST-FIT STRATEGY\n");
    printf("        COMPILER DESIGN LABORATORY\n");
    printf("========================================================================\n");

    Heap heap;
    heap.block_count = 1;
    heap.blocks[0].address = 0;
    heap.blocks[0].size = HEAP_SIZE;
    heap.blocks[0].allocated = false;
    strcpy(heap.blocks[0].name, "Free");

    printf("\nInitial Heap: 1024 bytes free\n");

    int step = 0;
    display_heap(&heap, step++);

    printf("\n");
    printf("========================================================================\n");
    printf("SIMULATING MALLOC/FREE OPERATIONS:\n");
    printf("========================================================================\n");

    malloc_sim(&heap, 64, "x");
    display_heap(&heap, step++);

    malloc_sim(&heap, 128, "y");
    display_heap(&heap, step++);

    malloc_sim(&heap, 96, "z");
    display_heap(&heap, step++);

    free_sim(&heap, "y");
    display_heap(&heap, step++);

    malloc_sim(&heap, 100, "w");
    display_heap(&heap, step++);

    free_sim(&heap, "x");
    display_heap(&heap, step++);

    free_sim(&heap, "z");
    display_heap(&heap, step++);

    printf("\n");
    printf("========================================================================\n");
    printf("FRAGMENTATION ANALYSIS:\n");
    printf("========================================================================\n");

    int total_free = 0, free_blocks = 0;
    for (int i = 0; i < heap.block_count; i++) {
        if (!heap.blocks[i].allocated) {
            total_free += heap.blocks[i].size;
            free_blocks++;
        }
    }

    printf("\nFree memory: %d bytes in %d block(s)\n", total_free, free_blocks);
    printf("Fragmentation ratio: %.2f%%\n", 
           (float)(free_blocks - 1) / free_blocks * 100);

    printf("\n");
    printf("------------------------------------------------------------------------\n");
    printf("FIRST-FIT STRATEGY CHARACTERISTICS:\n");
    printf("------------------------------------------------------------------------\n");
    printf("✓ Simple and fast (O(n) per allocation)\n");
    printf("✓ Reasonable performance for many workloads\n");
    printf("✗ External fragmentation over time\n");
    printf("✗ Poor cache locality (allocate at start of list)\n");

    printf("\nAlternative strategies:\n");
    printf("  - Best-fit: slower but less fragmentation\n");
    printf("  - Worst-fit: allocate largest block\n");
    printf("  - Buddy system: power-of-2 block sizes\n");

    printf("\nStatus: Heap allocation simulation completed.\n");

    return 0;
}
