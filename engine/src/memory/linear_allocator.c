#include "memory/linear_allocator.h"

#include "core/kmemory.h"
#include "core/logger.h"

void linear_allocator_create(u64 total_size, void* memory, linear_allocator* out_allocator) {
    if (out_allocator) {
        out_allocator->total_size = total_size;
        out_allocator->allocated = 0;
        out_allocator->owns_memory = memory == 0;
        if (memory) {
            out_allocator->memory = memory;
        } else {
            out_allocator->memory = kallocate(total_size, MEMORY_TAG_LINEAR_ALLOCATOR);
        }
        // Initialise all values inside memory to 0 to clean memory up
        // Added benefit of warming up cache
        kzero_memory(out_allocator->memory, total_size);
    }
}

void linear_allocator_destroy(linear_allocator* allocator) {
    if (allocator) {
        if (allocator->owns_memory && allocator->memory) {
            kfree(allocator->memory, allocator->total_size, MEMORY_TAG_LINEAR_ALLOCATOR);
        }
        allocator->total_size = 0;
        allocator->allocated = 0;
        allocator->memory = 0;
        allocator->owns_memory = false;
    }
}

void* linear_allocator_allocate(linear_allocator* allocator, u64 size) {
    if (allocator && allocator->memory) {
        if (allocator->allocated + size > allocator->total_size) {
            u64 remaining = allocator->total_size - allocator->allocated;
            KERROR("Linear allocator has been asked to allocate more memory than is available, tried to allocate %lluB, only remaining size in allocator: %lluB", size, remaining);
            return 0;
        }
        void* block = allocator->memory + allocator->allocated;
        allocator->allocated += size;
        return block;
    }

    KERROR("linear_allocator_allocate - provided allocator not initialised");
    return 0;
}

void linear_allocator_free_all(linear_allocator* allocator) {
    if (allocator && allocator->memory) {
        allocator->allocated = 0;
        kzero_memory(allocator->memory, allocator->total_size);
    }
}
