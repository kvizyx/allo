#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include <stdint.h>
#include <sys/types.h>

#define DEFAULT_ALLOC_CAP 4 * 1024 // 4 KB

typedef struct
{
    uintptr_t ptr;
    size_t    offset;
    size_t    cap;
} linear_allocator_t;

linear_allocator_t linear_init(const size_t cap);
void* linear_alloc(linear_allocator_t* allocator, const size_t size);
void linear_realloc(linear_allocator_t* allocator);
void linear_free_all(linear_allocator_t* allocator);

static size_t calculate_alignment(const uintptr_t addr, const size_t aligner);


#endif