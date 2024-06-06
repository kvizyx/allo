#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <stdio.h>

#include "linear_allocator.h"

linear_allocator_t linear_init(const size_t cap) {
    linear_allocator_t allocator = {0};

    allocator.cap = cap <= 0 ? DEFAULT_ALLOC_CAP : cap;
    allocator.ptr = (uintptr_t)memalign(allocator.cap, sizeof(uintptr_t));

    return allocator;
}

void* linear_alloc(linear_allocator_t* allocator, const size_t size) {
    assert(allocator);

    size_t alloc_size = size;

    #ifndef LINEAR_ALLOC_NOALIGN
        alloc_size += calculate_alignment(allocator->ptr + (allocator->offset + size), sizeof(uintptr_t));
    #endif

    if ((allocator->offset + alloc_size) > allocator->cap) {
        // TODO: allocate more memory
        return NULL;
    }

    void* region_start_p = (void*)(allocator->ptr + allocator->offset);

    allocator->offset += alloc_size;

    return region_start_p;
}

void linear_reset(linear_allocator_t* allocator) {
    assert(allocator);
    allocator->offset = 0;
}

void linear_free_all(linear_allocator_t* allocator) {
    assert(allocator);

    free((void*)allocator->ptr);
    allocator->cap = 0;
    allocator->offset = 0;
}

static size_t calculate_alignment(const uintptr_t addr, const size_t aligner) {
    if ((addr % aligner) == 0) {
        return 0;
    }

    const size_t factor = (addr / aligner) + 1;
    const size_t aligned_address = factor * aligner;
    
    return aligned_address - addr;
}