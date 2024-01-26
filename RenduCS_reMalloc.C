#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct block {
    size_t size;
    int free;
    struct block *next;
} block_t;

typedef struct {
    int x;
    float y;
} custom_struct;

#define BLOCK_SIZE sizeof(block_t)

block_t *freeList = NULL;

block_t *find_free_block(size_t size) {
    block_t *current = freeList;
    while (current && !(current->free && current->size >= size)) {
        current = current->next;
    }
    return current;
}

block_t *request_space(block_t* last, size_t size) {
    block_t *block = (block_t *)sbrk(0);
    void *request = sbrk(size + BLOCK_SIZE);
    if (request == (void*) -1) {
        return NULL;
    }

    if (last) {
        last->next = block;
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;
    return block;
}

void *myMalloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }

    block_t *block;

    if (!freeList) {
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        freeList = block;
    } else {
        block_t *last = freeList;
        block = find_free_block(size);
        if (!block) {
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        } else {
            block->free = 0;
        }
    }

    return (block+1);
}

void free(void *ptr) {
    if (!ptr) {
        return;
    }

    block_t *block_ptr = (block_t*)ptr - 1;
    block_ptr->free = 1;

    block_t *current = freeList;
    while (current) {
        if (current->free && current->next && current->next->free) {
            current->size += current->next->size + BLOCK_SIZE;
            current->next = current->next->next;
        }
        current = current->next;
    }

    block_t *last = freeList;
    while (last && last->next) {
        last = last->next;
    }

    if (last && last->free) {
        if (brk(last) == -1) {
        }

        if (last == freeList) {
            freeList = NULL;
        } else {
            block_t *prev = freeList;
            while (prev && prev->next != last) {
                prev = prev->next;
            }
            if (prev) {
                prev->next = NULL;
            }
        }
    }
}

void *myCalloc(size_t num, size_t size) {
    size_t totalSize = num * size;
    void *ptr = myMalloc(totalSize);

    if (ptr) {
        memset(ptr, 0, totalSize);
    }

    return ptr;
}

void *myRealloc(void *oldPtr, size_t newSize) {
    if (newSize == 0) {
        if (oldPtr) {
            free(oldPtr);
        }
        return NULL;
    }

    if (!oldPtr) {
        return myMalloc(newSize);
    }

    block_t *block_ptr = (block_t*)oldPtr - 1;
    void *heapEnd = sbrk(0);
    if ((char*)block_ptr + block_ptr->size + BLOCK_SIZE == heapEnd) {
        intptr_t sizeDiff = newSize - block_ptr->size;

        if (sbrk(sizeDiff) == (void*) -1) {
            return NULL;
        }

        block_ptr->size = newSize;
        return oldPtr;
    }

    void *newPtr = myMalloc(newSize);
    if (!newPtr) {
        return NULL;
    }

    for (size_t i = 0; i < block_ptr->size && i < newSize; ++i) {
        ((char*)newPtr)[i] = ((char*)oldPtr)[i];
    }

    free(oldPtr);
    return newPtr;
}

int main() {
    printf("Testing myMalloc...\n");
    int *int_ptr = (int *)myMalloc(sizeof(int));
    *int_ptr = 10;
    printf("Allocated int: %d\n", *int_ptr);

    custom_struct *struct_ptr = (custom_struct *)myMalloc(sizeof(custom_struct));
    struct_ptr->x = 5;
    struct_ptr->y = 3.14;
    printf("Allocated custom_struct: {x: %d, y: %f}\n", struct_ptr->x, struct_ptr->y);

    printf("Testing myCalloc...\n");
    int *array_ptr = (int *)myCalloc(5, sizeof(int));
    for (int i = 0; i < 5; i++) {
        assert(array_ptr[i] == 0);
        array_ptr[i] = i * i;
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", array_ptr[i]);
    }
    printf("\n");

    printf("Testing myRealloc...\n");
    array_ptr = (int *)myRealloc(array_ptr, 10 * sizeof(int));
    for (int i = 5; i < 10; i++) {
        array_ptr[i] = i * i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", array_ptr[i]);
    }
    printf("\n");

    printf("Testing free...\n");
    free(int_ptr);
    free(struct_ptr);
    free(array_ptr);

    printf("All tests completed.\n");

    return 0;
}
