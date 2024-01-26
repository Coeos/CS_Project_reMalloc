# CS_Project_reMalloc


# Custom Memory Management in C

## Overview
This project implements a basic memory management system in C. It provides custom functions for dynamic memory allocation and deallocation, similar to the standard `malloc`, `free`, `calloc`, and `realloc` functions in C. The memory allocator uses a linked list to keep track of free memory blocks.

## Key Components

### Data Structures
- `block_t`: A structure representing a block of memory. Each block contains the size of the block, a flag indicating whether it is free, and a pointer to the next block in the free list.

### Functions
- `myMalloc(size_t size)`: Allocates `size` bytes of memory and returns a pointer to the allocated memory.
- `free(void *ptr)`: Deallocates the memory previously allocated by `myMalloc`, `myCalloc`, or `myRealloc`.
- `myCalloc(size_t num, size_t size)`: Allocates memory for an array of `num` elements of `size` bytes each and initializes all bytes to zero.
- `myRealloc(void *oldPtr, size_t newSize)`: Resizes the memory block pointed to by `oldPtr` to `newSize` bytes.

### Internal Helper Functions
- `find_free_block(size_t size)`: Searches the free list for a block of memory of at least `size` bytes.
- `request_space(block_t* last, size_t size)`: Requests a new block of memory from the operating system.

### Usage
1. Call `myMalloc` to allocate memory.
2. Use `free` to deallocate memory.
3. Use `myCalloc` for zero-initialized memory allocation.
4. Use `myRealloc` to resize an allocated memory block.

## Testing
The `main` function includes tests for `myMalloc`, `free`, `myCalloc`, and `myRealloc`.

## Notes
- This implementation is a basic example and is not as efficient or robust as the standard memory management functions in C.
- It is intended for educational purposes to understand how memory allocation works at a lower level.
